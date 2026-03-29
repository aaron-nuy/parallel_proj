//
// Created by aaron on 17/03/2026.
//

#ifndef PARALLEL_PROJ_INDIVIDUAL_H
#define PARALLEL_PROJ_INDIVIDUAL_H
#include <stdlib.h>

#include "typedefs.h"

typedef enum {
    Susceptible = (u8) 0,
    Exposed,
    Infected,
    Recovered
} IndividualState;

typedef struct {
    IndividualState *states;
    IndividualState *previous_states;
    u32 *times_in_state;

    u32 *grid_poss_x;
    u32 *grid_poss_y;

    f64 *exposeds;
    f64 *infecteds;
    f64 *recovereds;
} IndividualSoA;

[[nodiscard]] IndividualSoA individual_soa_create(u32 total_individuals);

void individual_soa_destroy(IndividualSoA *soa);

#endif //PARALLEL_PROJ_INDIVIDUAL_H
