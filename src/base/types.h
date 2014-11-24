#pragma once

#include "base/platform.h"
#include <stdint.h>

typedef int8_t      i8;
typedef uint8_t     u8;
typedef int16_t     i16;
typedef uint16_t    u16;
typedef int32_t     i32;
typedef uint32_t    u32;
typedef int64_t     i64;
typedef uint64_t    u64;

typedef float       f32;
typedef double      f64;

#if defined(OS_ARCH_32)
typedef u32 uptr;
typedef i32 iptr;
#elif defined(OS_ARCH_64)
typedef u64 uptr;
typedef i64 iptr;
#endif
