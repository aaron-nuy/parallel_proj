//
// Created by aaron on 17/03/2026.
//

#include "rand_utils.h"
#include <math.h>
#include <float.h>
#include <stdio.h>


#define _RAND_DEG  31
#define _RAND_SEP   3

static i32 _rand_table[_RAND_DEG];
static int _rand_fptr = _RAND_SEP;
static int _rand_rptr = 0;

void rand_init(u32 seed)
{
    srand(seed);
}

[[nodiscard]] i32 rand_rand(void)
{
    return rand();
}

[[nodiscard]] i32 rand_i32_uniform(i32 min, i32 max)
{
    return min + rand() % (max - min + 1);;
}

[[nodiscard]] f64 rand_f64_uniform(f64 min, f64 max)
{
    return min + (max - min) * (rand_rand() / (f64) RAND_MAX);
}

[[nodiscard]] f64 rand_f64_uniform_01(void)
{
    return rand_rand() / (f64) RAND_MAX;
}

[[nodiscard]] f64 rand_f64_negexp(f64 mean)
{
    f64 u = rand_rand() / (f64) RAND_MAX;
    if (u >= 1.0)
        u = 1.0 - DBL_EPSILON;
    return -mean * log(1.0 - u);
}

u64 hash_u64(u64 x) {
    x ^= x >> 30;
    x *= 0xbf58476d1ce4e5b9ULL;
    x ^= x >> 27;
    x *= 0x94d049bb133111ebULL;
    x ^= x >> 31;
    return x;
}

[[nodiscard]] f64 rand_f64_uniform_01_stateless(u32 iteration, u32 id) {
    u64 seed = ((u64)iteration << 32) | id;
    u64 hash = hash_u64(seed);
    return (f64)hash / (f64)0xFFFFFFFFFFFFFFFFULL;
}

#ifndef SEQUENTIAL
[[nodiscard]] bool should_transition(u8 num_inf_neighbors, f64 prob_transition)
{
    if (num_inf_neighbors == 0) return false;
    return 1 - exp(-0.5 * num_inf_neighbors) > prob_transition;
}
#else
[[nodiscard]] bool should_transition(u8 num_inf_neighbors)
{
    if (num_inf_neighbors == 0) return false;
    return 1 - exp(-0.5 * num_inf_neighbors) > rand_f64_uniform_01();
}
#endif

