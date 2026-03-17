//
// Created by aaron on 17/03/2026.
//

#ifndef PARALLEL_PROJ_INDIVIDUAL_H
#define PARALLEL_PROJ_INDIVIDUAL_H
#include "typedefs.h"

const MIN_IND_ARR_CAPACITY = 5;

typedef enum {
    Susceptible = (u8)0,
    Exposed,
    Infected,
    Recovered
} IndividualState;

typedef struct Individual {
    IndividualState state;
    u32 time_in_state;

    // keep position in grid for fast modifying of grid
    u32 grid_pos_x, grid_pos_y;

    // every individual has random state durations but remain the same throughout the
    // entire simulation
    struct {
        const f64 exposed;
        const f64 infected;
        const f64 recovered;
    } state_duration;
} Individual;

typedef struct {
    u32* indices;
    u32 size;
    u32 capacity;
    const u32 MIN_CAPACITY;
} IndividualNode;

#endif //PARALLEL_PROJ_INDIVIDUAL_H