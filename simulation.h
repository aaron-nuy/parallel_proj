//
// Created by aaron on 17/03/2026.
//

#ifndef PARALLEL_PROJ_SIMULATION_H
#define PARALLEL_PROJ_SIMULATION_H
#include "stdio.h"
#include "grid.h"
#include "individual_soa.h"

typedef struct {
    u16 num_susceptible;
    u16 num_exposed;
    u16 num_infected;
    u16 num_recovered;
}
OutputData;

typedef struct {
    u32 total_individuals;
    u32 current_iter;
    u32 iterations_to_run;
    Grid grid;
    IndividualSoA individual_soa;
    OutputData* output_data;
} Simulation;

[[nodiscard]] Simulation simulation_create(u32 total_individuals, u32 iterations_to_run, u32 grid_width,
                                           u32 grid_height);

void simulation_populate(Simulation *simulation);

void simulation_run(Simulation *simulation);

void simulation_destroy(Simulation *simulation);

void simulation_step(Simulation *simulation);

void simulation_output_csv(Simulation *simulation);

void simulation_print_last(Simulation *simulation);

[[nodiscard]] u8 simulation_get_num_infected_neighbors(Simulation *simulation, u32 curr_x, u32 curr_y);

#endif //PARALLEL_PROJ_SIMULATION_H
