const mongoose = require('mongoose');

const postSchema = new mongoose.Schema(
  {
    title: {
      type: String,
      required: [true, 'Title is required'],
      trim: true,
      minlength: [3, 'Title must be at least 3 characters'],
      maxlength: [200, 'Title cannot exceed 200 characters'],
      index: true,
    },
    author: {
      type: String,
      required: [true, 'Author is required'],
      trim: true,
      maxlength: [100, 'Author name cannot exceed 100 characters'],
      index: true,
    },
    content: {
      type: String,
      required: [true, 'Content is required'],
      minlength: [10, 'Content must be at least 10 characters'],
    },
    tags: {
      type: [String],
      default: [],
      validate: {
        validator: (tags) => tags.length <= 20,
        message: 'A post can have at most 20 tags',
      },
      index: true,
    },
    slug: {
      type: String,
      unique: true,
      index: true,
    },
    published: {
      type: Boolean,
      default: false,
      index: true,
    },
    views: {
      type: Number,
      default: 0,
      min: 0,
    },
    summary: {
      type: String,
      maxlength: [500, 'Summary cannot exceed 500 characters'],
    },
  },
  {
    timestamps: true, // adds createdAt and updatedAt automatically
    toJSON: { virtuals: true },
    toObject: { virtuals: true },
  }
);

// Auto-generate slug from title before saving
postSchema.pre('save', function (next) {
  if (this.isModified('title') || this.isNew) {
    this.slug = slugify(this.title) + '-' + Date.now();
  }
  // Auto-generate summary from content if not provided
  if (!this.summary && this.content) {
    this.summary = this.content.substring(0, 200).trimEnd() + (this.content.length > 200 ? '...' : '');
  }
  next();
});

// Sanitise tags: lowercase, trim, remove duplicates
postSchema.pre('save', function (next) {
  if (this.isModified('tags')) {
    this.tags = [...new Set(this.tags.map((t) => t.toLowerCase().trim()).filter(Boolean))];
  }
  next();
});

// Text index for full-text search
postSchema.index({ title: 'text', content: 'text', tags: 'text' });

// Compound index for common list queries
postSchema.index({ published: 1, createdAt: -1 });
postSchema.index({ author: 1, createdAt: -1 });

function slugify(text) {
  return text
    .toLowerCase()
    .replace(/[^\w\s-]/g, '')
    .replace(/\s+/g, '-')
    .replace(/-+/g, '-')
    .trim();
}

module.exports = mongoose.model('Post', postSchema);
