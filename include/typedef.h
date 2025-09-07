#ifndef TYPEDEFS_H
#define TYPEDEFS_H

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

// ---- Basic Aliases ----
typedef uint8_t   uint8;
typedef int8_t    int8;
typedef uint16_t  uint16;
typedef int16_t   int16;
typedef uint32_t  uint32;
typedef int32_t   int32;
typedef uint64_t  uint64;
typedef int64_t   int64;

typedef float     float32;
typedef double    float64;

typedef size_t    usize;
typedef ptrdiff_t isize;

// ---- Compile-time Assertions ----
#define COMPILE_ASSERT(name, expr) \
    typedef char name[(expr) ? 1 : -1]

// ---- Size Checks ----
COMPILE_ASSERT(uint8_size,   sizeof(uint8)  == 1);
COMPILE_ASSERT(int8_size,   sizeof(int8)  == 1);

COMPILE_ASSERT(uint16_size,  sizeof(uint16) == 2);
COMPILE_ASSERT(int16_size,  sizeof(int16) == 2);

COMPILE_ASSERT(uint32_size,  sizeof(uint32) == 4);
COMPILE_ASSERT(int32_size,  sizeof(int32) == 4);

COMPILE_ASSERT(uint64_size,  sizeof(uint64) == 8);
COMPILE_ASSERT(int64_size,  sizeof(int64) == 8);

COMPILE_ASSERT(float32_size,  sizeof(float32) == 4);
COMPILE_ASSERT(float64_size,  sizeof(float64) == 8);

#endif // TYPEDEFS_H

