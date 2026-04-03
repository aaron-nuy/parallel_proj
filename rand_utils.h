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

[[nodiscard]] i32 rand_i32_0_300();

[[nodiscard]] f64 rand_f64_uniform(f64 min, f64 max);

[[nodiscard]] f64 rand_f64_uniform_01();

[[nodiscard]] f64 rand_f64_negexp(f64 mean);

#ifndef SEQUENTIAL
[[nodiscard]] bool should_transition(u8 num_inf_neighbors, f64 prob_transition);
#else
[[nodiscard]] bool should_transition(u8 num_inf_neighbors);
#endif

#endif //PARALLEL_PROJ_RAND_UTILS_H
