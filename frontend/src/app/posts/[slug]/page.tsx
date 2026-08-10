'use client';

import { useState } from 'react';
import { useParams, useRouter } from 'next/navigation';
import useSWR from 'swr';
import { postsApi, Post } from '../../../lib/api';

export default function PostDetailPage() {
  const { slug } = useParams<{ slug: string }>();
  const router = useRouter();
  const [deleting, setDeleting] = useState(false);

  // Try slug first, fall back to ID
  const { data: post, error, isLoading } = useSWR<Post>(
    slug ? `post:${slug}` : null,
    () => postsApi.getBySlug(slug).catch(() => postsApi.getById(slug))
  );

  const handleDelete = async () => {
    if (!post) return;
    if (!confirm('Delete this post permanently?')) return;
    setDeleting(true);
    try {
      await postsApi.delete(post._id);
      router.push('/');
    } catch {
      alert('Failed to delete post');
      setDeleting(false);
    }
  };

  if (isLoading) return <div className="loading-center"><div className="spinner" /></div>;
  if (error) return <div className="alert alert-error">Post not found or failed to load.</div>;
  if (!post) return null;

  return (
    <article className="post-detail">
      <div style={{ display: 'flex', alignItems: 'center', gap: '1rem', marginBottom: '1.5rem' }}>
        <a href="/" className="btn btn-ghost">← Back</a>
        <a href={`/posts/${post._id}/edit`} className="btn btn-ghost">Edit</a>
        <button className="btn btn-danger" onClick={handleDelete} disabled={deleting}>
          {deleting ? 'Deleting...' : 'Delete'}
        </button>
      </div>

      {!post.published && (
        <div className="alert" style={{ background: '#fef9c3', color: '#854d0e', marginBottom: '1rem' }}>
          This post is a draft and not publicly visible.
        </div>
      )}

      <h1>{post.title}</h1>

      <div className="post-meta" style={{ marginTop: '0.5rem', marginBottom: '1.5rem' }}>
        <span>By <strong>{post.author}</strong></span>
        <span>{new Date(post.createdAt).toLocaleDateString('en-US', { year: 'numeric', month: 'long', day: 'numeric' })}</span>
        <span>Updated {new Date(post.updatedAt).toLocaleDateString()}</span>
        <span>👁 {post.views} views</span>
      </div>

      {post.tags.length > 0 && (
        <div className="tags" style={{ marginBottom: '1.5rem' }}>
          {post.tags.map((tag) => (
            <a key={tag} href={`/?tag=${tag}`} className="tag">{tag}</a>
          ))}
        </div>
      )}

      <hr style={{ borderColor: 'var(--border)', marginBottom: '2rem' }} />

      <div className="post-content">{post.content}</div>
    </article>
  );
}
