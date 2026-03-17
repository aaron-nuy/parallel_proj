//
// Created by aaron on 17/03/2026.
//

#include "individual.h"

[[nodiscard]] static inline Individual individual_create(IndividualState state, u32 time_in_state, u32 grid_pos_x,
                                                         u32 grid_pos_y, f64 duration_exposed, f64 duration_infected,
                                                         f64 duration_recovered)
{
    return (Individual)
    {
        .
        state = state,
        .
        time_in_state = time_in_state,
        .
        grid_pos_x = grid_pos_x,
        .
        grid_pos_y = grid_pos_y,
        .
        state_duration = {
            .exposed = duration_exposed, .infected = duration_infected,
            .recovered = duration_recovered
        }
    };
}