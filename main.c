#include <stdio.h>
#include "rand_utils.h"
#include "simulation.h"
#include "typedefs.h"
#include <time.h>

#ifdef PARALLEL
#include <mpi.h>
#endif

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

#ifdef SEQUENTIAL
int main(int argc, char **argv)
{
    rand_init(SEED);

    u64 start_cycles, end_cycles;
    struct timespec start_time, end_time;
    f64 elapsed_time;

    auto simulation = simulation_create(TOTAL_INDIVIDUALS, ITERATIONS_TO_RUN, GRID_WIDTH, GRID_HEIGHT);
    simulation_populate(&simulation);

    clock_gettime(CLOCK_MONOTONIC, &start_time);
    start_cycles = rdtsc();

    simulation_run(&simulation);

    end_cycles = rdtsc();
    clock_gettime(CLOCK_MONOTONIC, &end_time);
    elapsed_time =
            (end_time.tv_sec - start_time.tv_sec) +
            (end_time.tv_nsec - start_time.tv_nsec) / 1e9;

    simulation_output_csv(&simulation);
    simulation_print_last(&simulation);

    simulation_destroy(&simulation);

    printf("\n");
    printf("Cycles taken to simulate were: %lu cycles\n", (u64) (end_cycles - start_cycles));
    printf("Time taken to simulate was: %f seconds\n", elapsed_time);


    return 0;
}
#else
int main(int argc, char **argv)
{
    rand_init(SEED);

    u32 world_rank, world_size;

    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &world_size);
    MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);

    Simulation simulation;
    u64 start_cycles, end_cycles;
    struct timespec start_time, end_time;
    f64 elapsed_time;

    simulation = simulation_create(TOTAL_INDIVIDUALS, ITERATIONS_TO_RUN, GRID_WIDTH, GRID_HEIGHT);
    simulation_populate(&simulation);

    if (world_rank == 0)
    {
        clock_gettime(CLOCK_MONOTONIC, &start_time);
        start_cycles = rdtsc();
    }

    simulation_run(&simulation);

    if (world_rank == 0)
    {
        end_cycles = rdtsc();
        clock_gettime(CLOCK_MONOTONIC, &end_time);
        elapsed_time =
                (end_time.tv_sec - start_time.tv_sec) +
                (end_time.tv_nsec - start_time.tv_nsec) / 1e9;
    }

    simulation_destroy(&simulation);

    MPI_Finalize();

    if (world_rank == 0)
    {
        printf("Cycles taken to simulate were: %lu cycles\n", (u64) (end_cycles - start_cycles));
        printf("Time taken to simulate was: %f seconds\n", elapsed_time);
    }

    return 0;
}
#endif
