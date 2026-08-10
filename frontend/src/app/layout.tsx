import type { Metadata } from 'next';
import './globals.css';

export const metadata: Metadata = {
  title: 'Blog Platform',
  description: 'A scalable blog platform built with Next.js, Node.js, MongoDB, and Redis',
};

export default function RootLayout({ children }: { children: React.ReactNode }) {
  return (
    <html lang="en">
      <body>
        <header className="site-header">
          <div className="container">
            <a href="/" className="site-logo">BlogPlatform</a>
            <nav>
              <a href="/">Posts</a>
              <a href="/posts/new">Write</a>
            </nav>
          </div>
        </header>
        <main className="container">{children}</main>
        <footer className="site-footer">
          <div className="container">
            <p>Built with Next.js · Node.js · MongoDB · Redis</p>
          </div>
        </footer>
      </body>
    </html>
  );
}
