//
// Created by aaron on 17/03/2026.
//

#ifndef PARALLEL_PROJ_TYPEDEFS_H
#define PARALLEL_PROJ_TYPEDEFS_H
#include <stdint.h>

typedef float f32;
typedef double f64;

typedef int64_t i64;
typedef int32_t i32;
typedef int16_t i16;
typedef int8_t i8;

typedef uint64_t u64;
typedef uint32_t u32;
typedef uint16_t u16;
typedef uint8_t u8;

#if defined(__STDC_VERSION__) && __STDC_VERSION__ >= 199901L
#include <stdbool.h>
#else
typedef u8 bool;
#define true 1
#define false 0
#endif

#endif //PARALLEL_PROJ_TYPEDEFS_H
