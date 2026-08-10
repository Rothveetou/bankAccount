const API_BASE = process.env.NEXT_PUBLIC_API_URL || 'http://localhost:4000/api';

export interface Post {
  _id: string;
  title: string;
  author: string;
  content: string;
  tags: string[];
  slug: string;
  published: boolean;
  views: number;
  summary?: string;
  createdAt: string;
  updatedAt: string;
}

export interface PostsResponse {
  data: Post[];
  pagination: {
    total: number;
    page: number;
    limit: number;
    pages: number;
  };
}

export interface CreatePostInput {
  title: string;
  author: string;
  content: string;
  tags?: string[];
  published?: boolean;
  summary?: string;
}

async function apiFetch<T>(path: string, options?: RequestInit): Promise<T> {
  const res = await fetch(`${API_BASE}${path}`, {
    headers: { 'Content-Type': 'application/json' },
    ...options,
  });
  if (!res.ok) {
    const body = await res.json().catch(() => ({}));
    throw new Error(body.error || `HTTP ${res.status}`);
  }
  return res.json();
}

export const postsApi = {
  list: (params?: Record<string, string>) => {
    const qs = params ? '?' + new URLSearchParams(params).toString() : '';
    return apiFetch<PostsResponse>(`/posts${qs}`);
  },
  getById: (id: string) => apiFetch<Post>(`/posts/${id}`),
  getBySlug: (slug: string) => apiFetch<Post>(`/posts/slug/${slug}`),
  create: (data: CreatePostInput) =>
    apiFetch<Post>('/posts', { method: 'POST', body: JSON.stringify(data) }),
  update: (id: string, data: Partial<CreatePostInput>) =>
    apiFetch<Post>(`/posts/${id}`, { method: 'PUT', body: JSON.stringify(data) }),
  delete: (id: string) => apiFetch<{ message: string }>(`/posts/${id}`, { method: 'DELETE' }),
};
