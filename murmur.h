#pragma once

#include <stdint.h>

uint64_t murmur_hash_64a(const void *key, int len, uint64_t seed);
