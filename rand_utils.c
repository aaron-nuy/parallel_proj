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
    if (seed == 0u)
        seed = 1u;

    _rand_table[0] = (i32) seed;

    for (int i = 1; i < _RAND_DEG; i++)
    {
        i32 prev = _rand_table[i - 1];
        i32 hi = prev / 127773;
        i32 lo = prev % 127773;
        i32 x = 16807 * lo - 2836 * hi;
        if (x < 0)
            x += 2147483647;
        _rand_table[i] = x;
    }

    _rand_fptr = _RAND_SEP;
    _rand_rptr = 0;

    for (int i = _RAND_DEG * 10; i > 0; i--)
        (void) rand_rand();
}

[[nodiscard]] i32 rand_rand(void)
{
    _rand_table[_rand_fptr] += _rand_table[_rand_rptr];

    i32 result = (_rand_table[_rand_fptr] >> 1) & 0x7FFFFFFF;

    if (++_rand_fptr >= _RAND_DEG)
    {
        _rand_fptr = 0;
        _rand_rptr++;
    } else if (++_rand_rptr >= _RAND_DEG)
    {
        _rand_rptr = 0;
    }

    return result;
}

[[nodiscard]] i32 rand_i32_uniform(i32 min, i32 max)
{
    return min + (i32)(((u64)(u32)rand_rand() * (u32)(max - min + 1)) >> 32);
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

