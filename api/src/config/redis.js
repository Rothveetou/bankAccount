const { createClient } = require('redis');

let client;

async function connectRedis() {
  client = createClient({ url: process.env.REDIS_URL || 'redis://localhost:6379' });

  client.on('error', (err) => console.error('Redis error:', err));
  client.on('connect', () => console.log('Redis connected'));

  await client.connect();
  return client;
}

function getRedisClient() {
  if (!client) throw new Error('Redis client not initialised');
  return client;
}

module.exports = { connectRedis, getRedisClient };
