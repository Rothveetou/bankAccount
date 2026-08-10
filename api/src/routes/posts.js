const express = require('express');
const router = express.Router();
const Post = require('../models/Post');
const cache = require('../services/cache');
const { validatePost, validateUpdate } = require('../middleware/validate');

// GET /posts — list with pagination, filtering by tag/author, full-text search
router.get('/', async (req, res) => {
  try {
    const {
      page = 1,
      limit = 10,
      tag,
      author,
      q,
      published,
    } = req.query;

    const pageNum = Math.max(1, parseInt(page));
    const limitNum = Math.min(50, Math.max(1, parseInt(limit)));
    const skip = (pageNum - 1) * limitNum;

    const cacheKey = `posts:list:${JSON.stringify(req.query)}`;
    const cached = await cache.get(cacheKey);
    if (cached) return res.json(cached);

    const filter = {};
    if (tag) filter.tags = tag.toLowerCase();
    if (author) filter.author = new RegExp(author, 'i');
    if (published !== undefined) filter.published = published === 'true';
    if (q) filter.$text = { $search: q };

    const [posts, total] = await Promise.all([
      Post.find(filter)
        .select('-content') // exclude heavy content from list view
        .sort({ createdAt: -1 })
        .skip(skip)
        .limit(limitNum)
        .lean(),
      Post.countDocuments(filter),
    ]);

    const result = {
      data: posts,
      pagination: {
        total,
        page: pageNum,
        limit: limitNum,
        pages: Math.ceil(total / limitNum),
      },
    };

    await cache.set(cacheKey, result, 60);
    res.json(result);
  } catch (err) {
    res.status(500).json({ error: err.message });
  }
});

// GET /posts/:id
router.get('/:id', async (req, res) => {
  try {
    const cacheKey = `posts:${req.params.id}`;
    const cached = await cache.get(cacheKey);
    if (cached) {
      // increment views in background without blocking response
      Post.findByIdAndUpdate(req.params.id, { $inc: { views: 1 } }).exec();
      return res.json(cached);
    }

    const post = await Post.findById(req.params.id).lean();
    if (!post) return res.status(404).json({ error: 'Post not found' });

    await Post.findByIdAndUpdate(req.params.id, { $inc: { views: 1 } });
    post.views = (post.views || 0) + 1;

    await cache.set(cacheKey, post, 120);
    res.json(post);
  } catch (err) {
    if (err.name === 'CastError') return res.status(404).json({ error: 'Post not found' });
    res.status(500).json({ error: err.message });
  }
});

// GET /posts/slug/:slug
router.get('/slug/:slug', async (req, res) => {
  try {
    const cacheKey = `posts:slug:${req.params.slug}`;
    const cached = await cache.get(cacheKey);
    if (cached) return res.json(cached);

    const post = await Post.findOne({ slug: req.params.slug }).lean();
    if (!post) return res.status(404).json({ error: 'Post not found' });

    await cache.set(cacheKey, post, 120);
    res.json(post);
  } catch (err) {
    res.status(500).json({ error: err.message });
  }
});

// POST /posts
router.post('/', validatePost, async (req, res) => {
  try {
    const post = await Post.create(req.body);
    await cache.invalidatePattern('posts:list:*');
    res.status(201).json(post);
  } catch (err) {
    if (err.code === 11000) return res.status(409).json({ error: 'Duplicate slug' });
    res.status(500).json({ error: err.message });
  }
});

// PUT /posts/:id
router.put('/:id', validateUpdate, async (req, res) => {
  try {
    const post = await Post.findByIdAndUpdate(req.params.id, req.body, {
      new: true,
      runValidators: true,
    });
    if (!post) return res.status(404).json({ error: 'Post not found' });

    await Promise.all([
      cache.del(`posts:${req.params.id}`),
      cache.del(`posts:slug:${post.slug}`),
      cache.invalidatePattern('posts:list:*'),
    ]);

    res.json(post);
  } catch (err) {
    if (err.name === 'CastError') return res.status(404).json({ error: 'Post not found' });
    res.status(500).json({ error: err.message });
  }
});

// DELETE /posts/:id
router.delete('/:id', async (req, res) => {
  try {
    const post = await Post.findByIdAndDelete(req.params.id);
    if (!post) return res.status(404).json({ error: 'Post not found' });

    await Promise.all([
      cache.del(`posts:${req.params.id}`),
      cache.del(`posts:slug:${post.slug}`),
      cache.invalidatePattern('posts:list:*'),
    ]);

    res.json({ message: 'Post deleted', id: req.params.id });
  } catch (err) {
    if (err.name === 'CastError') return res.status(404).json({ error: 'Post not found' });
    res.status(500).json({ error: err.message });
  }
});

module.exports = router;
