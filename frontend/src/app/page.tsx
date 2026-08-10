'use client';

import { useState, useCallback } from 'react';
import useSWR from 'swr';
import { postsApi, PostsResponse } from '../lib/api';
import PostCard from '../components/PostCard';

export default function HomePage() {
  const [page, setPage] = useState(1);
  const [search, setSearch] = useState('');
  const [tagFilter, setTagFilter] = useState('');
  const [query, setQuery] = useState({ page: '1', published: 'true' });

  const { data, error, isLoading } = useSWR<PostsResponse>(
    ['posts', query],
    () => postsApi.list(query),
    { keepPreviousData: true }
  );

  const handleSearch = useCallback(() => {
    const params: Record<string, string> = { page: '1', published: 'true' };
    if (search.trim()) params.q = search.trim();
    if (tagFilter.trim()) params.tag = tagFilter.trim().toLowerCase();
    setPage(1);
    setQuery(params);
  }, [search, tagFilter]);

  const handlePageChange = (p: number) => {
    setPage(p);
    setQuery((prev) => ({ ...prev, page: String(p) }));
  };

  return (
    <>
      <div style={{ display: 'flex', justifyContent: 'space-between', alignItems: 'center', marginBottom: '1.5rem' }}>
        <h1 style={{ fontSize: '1.75rem' }}>Latest Posts</h1>
        <a href="/posts/new" className="btn btn-primary">Write a post</a>
      </div>

      <div className="search-bar">
        <input
          placeholder="Search posts..."
          value={search}
          onChange={(e) => setSearch(e.target.value)}
          onKeyDown={(e) => e.key === 'Enter' && handleSearch()}
        />
        <input
          placeholder="Filter by tag"
          value={tagFilter}
          onChange={(e) => setTagFilter(e.target.value)}
          style={{ maxWidth: '160px' }}
          onKeyDown={(e) => e.key === 'Enter' && handleSearch()}
        />
        <button className="btn btn-primary" onClick={handleSearch}>Search</button>
        {(search || tagFilter) && (
          <button className="btn btn-ghost" onClick={() => { setSearch(''); setTagFilter(''); setPage(1); setQuery({ page: '1', published: 'true' }); }}>
            Clear
          </button>
        )}
      </div>

      {isLoading && <div className="loading-center"><div className="spinner" /></div>}
      {error && <div className="alert alert-error">Failed to load posts: {error.message}</div>}

      {data && (
        <>
          {data.data.length === 0 ? (
            <p style={{ color: 'var(--text-muted)' }}>No posts found.</p>
          ) : (
            data.data.map((post) => <PostCard key={post._id} post={post} />)
          )}

          {data.pagination.pages > 1 && (
            <div className="pagination">
              <button disabled={page === 1} onClick={() => handlePageChange(page - 1)}>Prev</button>
              {Array.from({ length: data.pagination.pages }, (_, i) => i + 1).map((p) => (
                <button key={p} className={p === page ? 'active' : ''} onClick={() => handlePageChange(p)}>{p}</button>
              ))}
              <button disabled={page === data.pagination.pages} onClick={() => handlePageChange(page + 1)}>Next</button>
            </div>
          )}
        </>
      )}
    </>
  );
}
