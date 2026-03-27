//
// Created by aaron on 17/03/2026.
//

#include "simulation.h"

#include <stdlib.h>

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
        .individuals = malloc(total_individuals * sizeof(Individual))
    };
}

void simulation_populate(Simulation *simulation)
{
    for (u32 i = 0; i < simulation->total_individuals; i++)
    {
        // TODO: Ask prof how to decide on which individuals should be the infected ones

        // Only pick 20 infected individuals
        IndividualState state = i % 1000 == 0 ? Infected : Susceptible;

        u32 x_pos = rand_i32_uniform(0, simulation->grid.width - 1);
        u32 y_pos = rand_i32_uniform(0, simulation->grid.height - 1);
        f64 de = rand_f64_negexp(3.0);
        f64 di = rand_f64_negexp(7.0);
        f64 dr = rand_f64_negexp(365.0);
        simulation->individuals[i] = individual_create(state, 0, x_pos, y_pos, de, di, dr);

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
    for (u32 i = 0; i < simulation->total_individuals; i++)
    {
        Individual *individual = &simulation->individuals[i];
        individual->previous_state = individual->state;

        // Manage cell cached infected individuals on state change
        switch (individual->state)
        {
            case Susceptible:
                const u32 num_infected_neighbors = simulation_get_num_infected_neighbors(
                    simulation, individual->grid_pos_x, individual->grid_pos_y);

                if (should_transition(num_infected_neighbors))
                {
                    individual->state = Exposed;
                    individual->time_in_state = 0;
                }
                break;
            case Exposed:
                if (individual->time_in_state > individual->state_duration.exposed)
                {
                    individual->state = Infected;
                    individual->time_in_state = 0;
                }
                break;
            case Infected:
                if (individual->time_in_state > individual->state_duration.infected)
                {
                    individual->state = Recovered;
                    individual->time_in_state = 0;
                }
                break;
            case Recovered:
                if (individual->time_in_state > individual->state_duration.recovered)
                {
                    individual->state = Susceptible;
                    individual->time_in_state = 0;
                }
                break;
        }

        individual->time_in_state++;
    }

    simulation_move_individuals_random(simulation);
}

void simulation_move_individuals_random(Simulation *simulation)
{
    for (u32 i = 0; i > simulation->total_individuals; i++)
    {
        // Handle old cell counter, must be computed using previous state and not current one
        grid_remove_individual(&simulation->grid, simulation->individuals[i].grid_pos_x,
                               simulation->individuals[i].grid_pos_y,
                               simulation->individuals[i].previous_state == Infected);


        simulation->individuals[i].grid_pos_x = rand_i32_uniform(0, simulation->grid.width - 1);
        simulation->individuals[i].grid_pos_y = rand_i32_uniform(0, simulation->grid.height - 1);
        grid_add_individual(&simulation->grid, simulation->individuals[i].grid_pos_x,
                            simulation->individuals[i].grid_pos_y, simulation->individuals[i].state == Infected);
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
    const u32 w = simulation->grid.width;
    const u32 h = simulation->grid.height;

    for (i32 i = -1; i <= 1; i++)
    {
        for (i32 j = -1; j <= 1; j++)
        {
            u32 curr_x = ((u32) ((i32) x + i) + w) % w;
            u32 curr_y = ((u32) ((i32) y + j) + h) % h;

            num_infected_neighbors += *grid_get_cell(&simulation->grid, curr_x, curr_y);
        }
    }

    return num_infected_neighbors;
}

void simulation_destroy(Simulation *simulation)
{
    grid_destroy(&simulation->grid);
    free(simulation->individuals);
}
