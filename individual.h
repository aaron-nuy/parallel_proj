//
// Created by aaron on 17/03/2026.
//

#ifndef PARALLEL_PROJ_INDIVIDUAL_H
#define PARALLEL_PROJ_INDIVIDUAL_H
#include "typedefs.h"

typedef enum {
    Susceptible = (u8) 0,
    Exposed,
    Infected,
    Recovered
} IndividualState;

typedef struct Individual {
    IndividualState state;
    IndividualState previous_state;
    u32 time_in_state;

    // keep position in the grid for fast access to indiviual position
    u32 grid_pos_x, grid_pos_y;

    // every individual has random state durations but remain the same throughout the
    // entire simulation
    struct {
        f64 exposed;
        f64 infected;
        f64 recovered;
    } state_duration;
} Individual;

[[nodiscard]] Individual individual_create(IndividualState state, u32 time_in_state, u32 grid_pos_x,
                                                         u32 grid_pos_y, f64 duration_exposed, f64 duration_infected,
                                                         f64 duration_recovered);

#endif //PARALLEL_PROJ_INDIVIDUAL_H