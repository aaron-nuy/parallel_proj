//
// Created by aaron on 17/03/2026.
//

#ifndef PARALLEL_PROJ_SIMULATION_H
#define PARALLEL_PROJ_SIMULATION_H
#include "grid.h"
#include "individual.h"

typedef struct {
    u32 total_individuals;
    u32 current_iter;
    u32 iterations_to_run;
    Grid grid;
    Individual *individuals;
} Simulation;

[[nodiscard]] Simulation simulation_create(u32 total_individuals, u32 iteratoins_to_run, u32 grid_width,
                                           u32 grid_height);

void simulation_populate(Simulation *simulation);

void simulation_run(Simulation *simulation);

void simulation_destroy(Simulation *simulation);

void simulation_step(Simulation *simulation);

void simulation_move_individuals_random(Simulation *simulation);

[[nodiscard]] u32 simulation_get_num_infected_neighbors(Simulation *simulation, u32 x, u32 y);

#endif //PARALLEL_PROJ_SIMULATION_H
