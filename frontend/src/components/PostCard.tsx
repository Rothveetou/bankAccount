import { Post } from '../lib/api';

interface Props {
  post: Post;
}

export default function PostCard({ post }: Props) {
  return (
    <div className="post-card">
      <h2><a href={`/posts/${post.slug || post._id}`}>{post.title}</a></h2>
      <div className="post-meta">
        <span>By {post.author}</span>
        <span>{new Date(post.createdAt).toLocaleDateString('en-US', { year: 'numeric', month: 'short', day: 'numeric' })}</span>
        <span className="views">👁 {post.views ?? 0} views</span>
        {!post.published && <span style={{ color: '#f59e0b' }}>Draft</span>}
      </div>
      {post.summary && <p className="post-summary">{post.summary}</p>}
      {post.tags.length > 0 && (
        <div className="tags">
          {post.tags.map((tag) => (
            <a key={tag} href={`/?tag=${tag}`} className="tag">{tag}</a>
          ))}
        </div>
      )}
    </div>
  );
}
