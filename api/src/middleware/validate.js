const Joi = require('joi');

const postSchema = Joi.object({
  title: Joi.string().min(3).max(200).required(),
  author: Joi.string().min(1).max(100).required(),
  content: Joi.string().min(10).required(),
  tags: Joi.array().items(Joi.string().max(50)).max(20).default([]),
  published: Joi.boolean().default(false),
  summary: Joi.string().max(500).optional(),
});

const updateSchema = postSchema.fork(
  ['title', 'author', 'content'],
  (field) => field.optional()
);

function validatePost(req, res, next) {
  const { error, value } = postSchema.validate(req.body, { abortEarly: false });
  if (error) {
    return res.status(400).json({
      error: 'Validation failed',
      details: error.details.map((d) => d.message),
    });
  }
  req.body = value;
  next();
}

function validateUpdate(req, res, next) {
  const { error, value } = updateSchema.validate(req.body, { abortEarly: false });
  if (error) {
    return res.status(400).json({
      error: 'Validation failed',
      details: error.details.map((d) => d.message),
    });
  }
  req.body = value;
  next();
}

module.exports = { validatePost, validateUpdate };
