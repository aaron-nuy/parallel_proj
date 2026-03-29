//
// Created by aaron on 29/03/2026.
//

#include "individual_soa.h"

inline void individual_soa_destroy(IndividualSoA *soa)
{
    free(soa->states);
    free(soa->previous_states);
    free(soa->times_in_state);
    free(soa->grid_poss_x);
    free(soa->grid_poss_y);
    free(soa->exposeds);
    free(soa->infecteds);
    free(soa->recovereds);
}

IndividualSoA individual_soa_create(u32 total_individuals)
{
    return (IndividualSoA){
        .states = malloc(total_individuals * sizeof(IndividualState)),
        .previous_states = malloc(total_individuals * sizeof(IndividualState)),
        .times_in_state = malloc(total_individuals * sizeof(u32)),
        .grid_poss_x = malloc(total_individuals * sizeof(u32)),
        .grid_poss_y = malloc(total_individuals * sizeof(u32)),
        .exposeds = malloc(total_individuals * sizeof(f64)),
        .infecteds = malloc(total_individuals * sizeof(f64)),
        .recovereds = malloc(total_individuals * sizeof(f64))
    };
}