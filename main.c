#include <stdio.h>
#include "rand_utils.h"
#include "simulation.h"
#include "typedefs.h"


const u32 GRID_WIDTH = 300;
const u32 GRID_HEIGHT = 300;
const u32 TOTAL_INDIVIDUALS = 20000;
const u32 SEED = 42;
const u32 ITERATIONS_TO_RUN = 365;

[[nodiscard]] u64 rdtsc()
{
    unsigned int lo, hi;
    __asm__ __volatile__ ("rdtsc" : "=a" (lo), "=d" (hi));
    return ((u64) hi << 32) | lo;
}

int main(int argc, char **argv)
{
    rand_init(SEED);

    auto simulation = simulation_create(TOTAL_INDIVIDUALS, ITERATIONS_TO_RUN, GRID_WIDTH, GRID_HEIGHT);
    simulation_populate(&simulation);
    auto start = rdtsc();
    simulation_run(&simulation);
    auto end = rdtsc();
    simulation_destroy(&simulation);

    printf("Time taken to simulate was: %lu cycles\n", (u64) (end - start));

    return 0;
}
