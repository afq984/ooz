#include "murmur.h"

#define BIG_CONSTANT(x) (x##LLU)

//
//-----------------------------------------------------------------------------
// Block read - on little-endian machines this is a single load,
// while on big-endian or unknown machines the byte accesses should
// still get optimized into the most efficient instruction.
static inline uint32_t getblock(const uint32_t *p) {
#if defined(__BYTE_ORDER__) && (__BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__)
  return *p;
#else
  const uint8_t *c = (const uint8_t *)p;
  return (uint32_t)c[0] | (uint32_t)c[1] << 8 | (uint32_t)c[2] << 16 | (uint32_t)c[3] << 24;
#endif
}

static inline uint64_t getblock(const uint64_t *p) {
#if defined(__BYTE_ORDER__) && (__BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__)
  return *p;
#else
  const uint8_t *c = (const uint8_t *)p;
  return (uint64_t)c[0] | (uint64_t)c[1] << 8 | (uint64_t)c[2] << 16 | (uint64_t)c[3] << 24 | (uint64_t)c[4] << 32 |
         (uint64_t)c[5] << 40 | (uint64_t)c[6] << 48 | (uint64_t)c[7] << 56;
#endif
}

uint64_t murmur_hash_64a(const void *key, int len, uint64_t seed) {
  const uint64_t m = BIG_CONSTANT(0xc6a4a7935bd1e995);
  const int r = 47;

  uint64_t h = seed ^ (len * m);

  const uint64_t *data = (const uint64_t *)key;
  const uint64_t *end = data + (len / 8);

  while (data != end) {
    uint64_t k = getblock(data++);

    k *= m;
    k ^= k >> r;
    k *= m;

    h ^= k;
    h *= m;
  }

  const unsigned char *data2 = (const unsigned char *)data;

  switch (len & 7) {
  case 7:
    h ^= uint64_t(data2[6]) << 48;
  case 6:
    h ^= uint64_t(data2[5]) << 40;
  case 5:
    h ^= uint64_t(data2[4]) << 32;
  case 4:
    h ^= uint64_t(data2[3]) << 24;
  case 3:
    h ^= uint64_t(data2[2]) << 16;
  case 2:
    h ^= uint64_t(data2[1]) << 8;
  case 1:
    h ^= uint64_t(data2[0]);
    h *= m;
  };

  h ^= h >> r;
  h *= m;
  h ^= h >> r;

  return h;
}
