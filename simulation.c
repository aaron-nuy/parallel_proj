//
// Created by aaron on 17/03/2026.
//

#include "simulation.h"

#include "rand_utils.h"

[[nodiscard]] Simulation simulation_create(u32 total_individuals, u32 iteratoins_to_run, u32 grid_width,
                                           u32 grid_height)
{
    return (Simulation)
    {
        .total_individuals = total_individuals,
        .current_iter = 0,
        .iterations_to_run = iteratoins_to_run,
        .grid = grid_create(grid_width, grid_height),
        .individual_soa = individual_soa_create(total_individuals)
    };
}

void simulation_populate(Simulation *simulation)
{
    for (u32 i = 0; i < simulation->total_individuals; i++)
    {
        // Only pick 20 infected individuals
        IndividualState state = i % 1000 == 0 ? Infected : Susceptible;

        u32 x_pos = rand_i32_uniform(0, simulation->grid.width - 1);
        u32 y_pos = rand_i32_uniform(0, simulation->grid.height - 1);
        f64 de = rand_f64_negexp(3.0);
        f64 di = rand_f64_negexp(7.0);
        f64 dr = rand_f64_negexp(365.0);

        simulation->individual_soa.states[i] = state;
        simulation->individual_soa.previous_states[i] = state;
        simulation->individual_soa.times_in_state[i] = 0;

        simulation->individual_soa.grid_poss_x[i] = x_pos;
        simulation->individual_soa.grid_poss_y[i] = y_pos;

        simulation->individual_soa.exposeds[i] = de;
        simulation->individual_soa.infecteds[i] = di;
        simulation->individual_soa.recovereds[i] = dr;

        grid_add_individual(&simulation->grid, x_pos, y_pos, state == Infected);
    }
}

void simulation_run(Simulation *simulation)
{
    for (simulation->current_iter = 0; simulation->current_iter < simulation->iterations_to_run; simulation->
         current_iter++)
    {
        simulation_step(simulation);
    }
}

void simulation_step(Simulation *simulation)
{
    IndividualState *states = simulation->individual_soa.states;
    IndividualState *previous_states = simulation->individual_soa.previous_states;
    u32 *times_in_state = simulation->individual_soa.times_in_state;

    u32 *grid_poss_x = simulation->individual_soa.grid_poss_x;
    u32 *grid_poss_y = simulation->individual_soa.grid_poss_y;

    f64 *exposed = simulation->individual_soa.exposeds;
    f64 *infected = simulation->individual_soa.infecteds;
    f64 *recovered = simulation->individual_soa.recovereds;

    for (u32 i = 0; i < simulation->total_individuals; i++)
    {
        previous_states[i] = states[i];

        switch (states[i])
        {
            case Susceptible: {
                const u32 num_infected_neighbors = simulation_get_num_infected_neighbors(
                    simulation, grid_poss_x[i], grid_poss_y[i]);

                if (should_transition(num_infected_neighbors))
                {
                    states[i] = Exposed;
                    times_in_state[i] = 0;
                }
                break;
            }
            case Exposed:
                if (times_in_state[i] > exposed[i])
                {
                    states[i] = Infected;
                    times_in_state[i] = 0;
                }
                break;
            case Infected:
                if (times_in_state[i] > infected[i])
                {
                    states[i] = Recovered;
                    times_in_state[i] = 0;
                }
                break;
            case Recovered:
                if (times_in_state[i] > recovered[i])
                {
                    states[i] = Susceptible;
                    times_in_state[i] = 0;
                }
                break;
        }

        times_in_state[i]++;
    }

    for (u32 i = 0; i < simulation->total_individuals; i++)
    {
        grid_remove_individual(&simulation->grid, grid_poss_x[i],
                               grid_poss_y[i],
                               previous_states[i] == Infected);

        grid_poss_x[i] = rand_i32_uniform(0, simulation->grid.width - 1);
        grid_poss_y[i] = rand_i32_uniform(0, simulation->grid.height - 1);

        grid_add_individual(&simulation->grid, grid_poss_x[i],
                            grid_poss_y[i], states[i] == Infected);
    }
}


/**
 * Shouldn't be called on non-susceptible Agent
 * @param simulation
 * @param x
 * @param y
 * @param index
 * @return
 */
[[nodiscard]] u32 simulation_get_num_infected_neighbors(Simulation *simulation, u32 x, u32 y)
{
    u32 num_infected_neighbors = 0;
    const u32 width = simulation->grid.width;
    const u32 height = simulation->grid.height;

    u32 wrapped_left_x = x == 0 ? width - 1 : x - 1;
    u32 wrapped_right_x = x == width - 1 ? 0 : x + 1;
    u32 wrapped_above_y = y == 0 ? height - 1 : y - 1;
    u32 wrapped_below_y = y == height - 1 ? 0 : y + 1;

    num_infected_neighbors += grid_get_cell_value(&simulation->grid, wrapped_left_x, wrapped_above_y);
    num_infected_neighbors += grid_get_cell_value(&simulation->grid, x, wrapped_above_y);
    num_infected_neighbors += grid_get_cell_value(&simulation->grid, wrapped_right_x, wrapped_above_y);
    num_infected_neighbors += grid_get_cell_value(&simulation->grid, wrapped_left_x, y);
    num_infected_neighbors += grid_get_cell_value(&simulation->grid, x, y);
    num_infected_neighbors += grid_get_cell_value(&simulation->grid, wrapped_right_x, y);
    num_infected_neighbors += grid_get_cell_value(&simulation->grid, wrapped_left_x, wrapped_below_y);
    num_infected_neighbors += grid_get_cell_value(&simulation->grid, x, wrapped_below_y);
    num_infected_neighbors += grid_get_cell_value(&simulation->grid, wrapped_right_x, wrapped_below_y);

    return num_infected_neighbors;
}

void simulation_destroy(Simulation *simulation)
{
    grid_destroy(&simulation->grid);
    individual_soa_destroy(&simulation->individual_soa);
}
