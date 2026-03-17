//
// Created by aaron on 17/03/2026.
//

#ifndef PARALLEL_PROJ_RAND_UTILS_H
#define PARALLEL_PROJ_RAND_UTILS_H
#include <stdlib.h>
#include <math.h>
#include "typedefs.h"

static inline void rand_init(u32 seed);

static inline i32 rand_rand();

[[nodiscard]] static inline i32 rand_i32_uniform(i32 min, i32 max);

[[nodiscard]] static inline f64 rand_f64_uniform(f64 min, f64 max);

[[nodiscard]] static inline f64 rand_f64_uniform_01();

[[nodiscard]] static inline f64 rand_f64_negexp(f64 mean);


#endif //PARALLEL_PROJ_RAND_UTILS_H
