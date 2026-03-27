//
// Created by aaron on 17/03/2026.
//

#ifndef PARALLEL_PROJ_RAND_UTILS_H
#define PARALLEL_PROJ_RAND_UTILS_H
#include <stdlib.h>
#include <math.h>
#include "typedefs.h"

void rand_init(u32 seed);

i32 rand_rand();

[[nodiscard]] i32 rand_i32_uniform(i32 min, i32 max);

[[nodiscard]] f64 rand_f64_uniform(f64 min, f64 max);

[[nodiscard]] f64 rand_f64_uniform_01();

[[nodiscard]] f64 rand_f64_negexp(f64 mean);

[[nodiscard]] bool should_transition(u32 num_inf_neighbors);


#endif //PARALLEL_PROJ_RAND_UTILS_H
