const { getRedisClient } = require('../config/redis');

const DEFAULT_TTL = 300; // 5 minutes

async function get(key) {
  const client = getRedisClient();
  const data = await client.get(key);
  return data ? JSON.parse(data) : null;
}

async function set(key, value, ttl = DEFAULT_TTL) {
  const client = getRedisClient();
  await client.setEx(key, ttl, JSON.stringify(value));
}

async function del(key) {
  const client = getRedisClient();
  await client.del(key);
}

async function invalidatePattern(pattern) {
  const client = getRedisClient();
  const keys = await client.keys(pattern);
  if (keys.length) await client.del(keys);
}

module.exports = { get, set, del, invalidatePattern };
