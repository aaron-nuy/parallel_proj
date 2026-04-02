#include "simulation.h"
#include <string.h>
#include "rand_utils.h"

[[nodiscard]] u8 simulation_get_num_infected_neighbors(const Simulation *simulation, u32 curr_x, u32 curr_y)
{
    const u32 width = simulation->grid.width;
    const u32 height = simulation->grid.height;

    u32 looped_left_x = (curr_x == 0) ? width - 1 : curr_x - 1;
    u32 looped_right_x = (curr_x == width - 1) ? 0 : curr_x + 1;
    u32 looped_up_y = (curr_y == 0) ? height - 1 : curr_y - 1;
    u32 looped_down_y = (curr_y == height - 1) ? 0 : curr_y + 1;

    u8 num_infected_neighbors = 0;

    num_infected_neighbors += grid_get_cell_value(&simulation->grid, looped_left_x, looped_up_y);
    num_infected_neighbors += grid_get_cell_value(&simulation->grid, curr_x, looped_up_y);
    num_infected_neighbors += grid_get_cell_value(&simulation->grid, looped_right_x, looped_up_y);
    num_infected_neighbors += grid_get_cell_value(&simulation->grid, looped_left_x, curr_y);
    num_infected_neighbors += grid_get_cell_value(&simulation->grid, curr_x, curr_y);
    num_infected_neighbors += grid_get_cell_value(&simulation->grid, looped_right_x, curr_y);
    num_infected_neighbors += grid_get_cell_value(&simulation->grid, looped_left_x, looped_down_y);
    num_infected_neighbors += grid_get_cell_value(&simulation->grid, curr_x, looped_down_y);
    num_infected_neighbors += grid_get_cell_value(&simulation->grid, looped_right_x, looped_down_y);
    return num_infected_neighbors;
}

void simulation_output_csv(Simulation *simulation)
{
    FILE *file = fopen("output.csv", "w");
    if (!file)
    {
        perror("fopen");
        return;
    }
    fprintf(file, "iteration,num_susceptible,num_exposed,num_infected,num_recovered\n");
    for (u32 i = 0; i < simulation->iterations_to_run; i++)
    {
        OutputData *d = &simulation->output_data[i];
        fprintf(file, "%u,%u,%u,%u,%u\n", i, d->num_susceptible, d->num_exposed, d->num_infected, d->num_recovered);
    }
    fclose(file);
}

void simulation_print_last(Simulation *simulation)
{
    u32 last = simulation->iterations_to_run - 1;
    OutputData *d = &simulation->output_data[last];
    printf("iteration=%u\nS=%u\nE=%u\nI=%u\nR=%u\n", last, d->num_susceptible, d->num_exposed, d->num_infected,
           d->num_recovered);
}

void simulation_destroy(Simulation *simulation)
{
    grid_destroy(&simulation->grid);
    individual_soa_destroy(&simulation->individual_soa);
    free(simulation->output_data);
}

#ifdef SEQUENTIAL

[[nodiscard]] Simulation simulation_create(u32 total_individuals, u32 iterations_to_run, u32 grid_width,
                                           u32 grid_height)
{
    OutputData *output_data = malloc(iterations_to_run * sizeof(OutputData));
    memset(output_data, 0, iterations_to_run * sizeof(OutputData));
    return (Simulation){
        .total_individuals = total_individuals,
        .current_iter = 0,
        .iterations_to_run = iterations_to_run,
        .grid = grid_create(grid_width, grid_height),
        .individual_soa = individual_soa_create(total_individuals),
        .output_data = output_data
    };
}

void simulation_populate(Simulation *simulation)
{
    for (u32 i = 0; i < simulation->total_individuals; i++)
    {
        // Only 20
        IndividualState state = (i % 1000 == 0) ? Infected : Susceptible;

        u32 x = rand_i32_uniform(0, simulation->grid.width - 1);
        u32 y = rand_i32_uniform(0, simulation->grid.height - 1);

        f64 de = rand_f64_negexp(3.0);
        f64 di = rand_f64_negexp(7.0);
        f64 dr = rand_f64_negexp(365.0);

        simulation->individual_soa.states[i] = state;
        simulation->individual_soa.previous_states[i] = state;

        simulation->individual_soa.times_in_state[i] = 0;

        simulation->individual_soa.grid_poss_x[i] = x;
        simulation->individual_soa.grid_poss_y[i] = y;

        simulation->individual_soa.exposeds[i] = de;
        simulation->individual_soa.infecteds[i] = di;
        simulation->individual_soa.recovereds[i] = dr;

        if (state == Infected)
            grid_add_individual(&simulation->grid, x, y);
    }
}

void simulation_step(Simulation *simulation)
{
    IndividualState *states = simulation->individual_soa.states;
    IndividualState *previous_states = simulation->individual_soa.previous_states;

    u32 *times_in_state = simulation->individual_soa.times_in_state;

    u32 *grid_poss_x = simulation->individual_soa.grid_poss_x;
    u32 *grid_poss_y = simulation->individual_soa.grid_poss_y;

    f64 *exposed_dur = simulation->individual_soa.exposeds;
    f64 *infected_dur = simulation->individual_soa.infecteds;
    f64 *recovered_dur = simulation->individual_soa.recovereds;

    OutputData *out = &simulation->output_data[simulation->current_iter];

    for (u32 i = 0; i < simulation->total_individuals; i++)
    {
        previous_states[i] = states[i];
        switch (states[i])
        {
            case Susceptible:
                out->num_susceptible++;
                const u8 num_inf_neighbors = simulation_get_num_infected_neighbors(
                    simulation, grid_poss_x[i], grid_poss_y[i]);
                if (should_transition(num_inf_neighbors))
                {
                    states[i] = Exposed;
                    times_in_state[i] = 0;
                }
                break;
            case Exposed:
                out->num_exposed++;
                if (times_in_state[i] > exposed_dur[i])
                {
                    states[i] = Infected;
                    times_in_state[i] = 0;
                }
                break;
            case Infected:
                out->num_infected++;
                if (times_in_state[i] > infected_dur[i])
                {
                    states[i] = Recovered;
                    times_in_state[i] = 0;
                }
                break;
            case Recovered:
                out->num_recovered++;
                if (times_in_state[i] > recovered_dur[i])
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
        if (previous_states[i] == Infected)
            grid_remove_individual(&simulation->grid, grid_poss_x[i], grid_poss_y[i]);

        grid_poss_x[i] = rand_i32_uniform(0, simulation->grid.width - 1);
        grid_poss_y[i] = rand_i32_uniform(0, simulation->grid.height - 1);

        if (states[i] == Infected)
            grid_add_individual(&simulation->grid, grid_poss_x[i], grid_poss_y[i]);
    }
}

#else

void calculate_local_ranges(u32 total, i32 rank, i32 size, u32 *output_start, u32 *output_count)
{
    u32 elements_per_proc = total / (u32) size;
    u32 extra_elements_to_last_proc = total % (u32) size;

    *output_start = (u32) rank * elements_per_proc;

    if (rank == size - 1)
        *output_count = elements_per_proc + extra_elements_to_last_proc;
    else
        *output_count = elements_per_proc;
}

[[nodiscard]] Simulation simulation_create(u32 total_individuals, u32 iterations_to_run, u32 grid_width,
                                           u32 grid_height, i32 world_rank, i32 world_size)
{
    OutputData *output_data = malloc(iterations_to_run * sizeof(OutputData));
    memset(output_data, 0, iterations_to_run * sizeof(OutputData));

    u32 local_start, local_count;
    calculate_local_ranges(total_individuals, world_rank, world_size, &local_start, &local_count);

    return (Simulation){
        .total_individuals = total_individuals,
        .current_iter = 0,
        .iterations_to_run = iterations_to_run,
        .grid = grid_create(grid_width, grid_height),
        .individual_soa = individual_soa_create(local_count),
        .output_data = output_data,
        .local_start = local_start,
        .local_count = local_count
    };
}

void simulation_populate(Simulation *simulation, i32 world_rank, i32 world_size)
{
    u32 grid_cells = simulation->grid.width * simulation->grid.height;
    if (world_rank == 0)
    {
        IndividualSoA all_individual_soa = individual_soa_create(simulation->total_individuals);
        for (u32 i = 0; i < simulation->total_individuals; i++)
        {
            // Only 20 infected
            IndividualState state = (i % 1000 == 0) ? Infected : Susceptible;

            u32 x = rand_i32_uniform(0, simulation->grid.width - 1);
            u32 y = rand_i32_uniform(0, simulation->grid.height - 1);

            f64 de = rand_f64_negexp(3.0);
            f64 di = rand_f64_negexp(7.0);
            f64 dr = rand_f64_negexp(365.0);

            all_individual_soa.states[i] = state;
            all_individual_soa.previous_states[i] = state;

            all_individual_soa.times_in_state[i] = 0;

            all_individual_soa.grid_poss_x[i] = x;
            all_individual_soa.grid_poss_y[i] = y;

            all_individual_soa.exposeds[i] = de;
            all_individual_soa.infecteds[i] = di;
            all_individual_soa.recovereds[i] = dr;

            if (state == Infected)
                grid_add_individual(&simulation->grid, x, y);
        }

        i32 *counts = malloc(world_size * sizeof(i32));
        i32 *displacements = malloc(world_size * sizeof(i32));

        u32 offset = 0;

        for (i32 process = 0; process < world_size; process++)
        {
            u32 start_idx, num_individuals;
            calculate_local_ranges(simulation->total_individuals, process, world_size, &start_idx, &num_individuals);

            counts[process] = (i32) num_individuals;
            displacements[process] = (i32) offset;

            offset += num_individuals;
        }

        MPI_Scatterv(all_individual_soa.states, counts, displacements, MPI_UNSIGNED_CHAR,
                     simulation->individual_soa.states,
                     (i32) simulation->local_count, MPI_UNSIGNED_CHAR, 0, MPI_COMM_WORLD);
        MPI_Scatterv(all_individual_soa.previous_states, counts, displacements, MPI_UNSIGNED_CHAR,
                     simulation->individual_soa.previous_states, (i32) simulation->local_count, MPI_UNSIGNED_CHAR, 0,
                     MPI_COMM_WORLD);
        MPI_Scatterv(all_individual_soa.times_in_state, counts, displacements, MPI_UNSIGNED,
                     simulation->individual_soa.times_in_state,
                     (i32) simulation->local_count, MPI_UNSIGNED, 0, MPI_COMM_WORLD);
        MPI_Scatterv(all_individual_soa.grid_poss_x, counts, displacements, MPI_UNSIGNED,
                     simulation->individual_soa.grid_poss_x,
                     (i32) simulation->local_count, MPI_UNSIGNED, 0, MPI_COMM_WORLD);
        MPI_Scatterv(all_individual_soa.grid_poss_y, counts, displacements, MPI_UNSIGNED,
                     simulation->individual_soa.grid_poss_y,
                     (i32) simulation->local_count, MPI_UNSIGNED, 0, MPI_COMM_WORLD);
        MPI_Scatterv(all_individual_soa.exposeds, counts, displacements, MPI_DOUBLE,
                     simulation->individual_soa.exposeds,
                     (i32) simulation->local_count, MPI_DOUBLE, 0, MPI_COMM_WORLD);
        MPI_Scatterv(all_individual_soa.infecteds, counts, displacements, MPI_DOUBLE,
                     simulation->individual_soa.infecteds,
                     (i32) simulation->local_count, MPI_DOUBLE, 0, MPI_COMM_WORLD);
        MPI_Scatterv(all_individual_soa.recovereds, counts, displacements, MPI_DOUBLE,
                     simulation->individual_soa.recovereds,
                     (i32) simulation->local_count, MPI_DOUBLE, 0, MPI_COMM_WORLD);
        individual_soa_destroy(&all_individual_soa);
        free(counts);
        free(displacements);
    } else
    {
        MPI_Scatterv(NULL, NULL, NULL, MPI_UNSIGNED_CHAR, simulation->individual_soa.states,
                     (i32) simulation->local_count, MPI_UNSIGNED_CHAR, 0, MPI_COMM_WORLD);
        MPI_Scatterv(NULL, NULL, NULL, MPI_UNSIGNED_CHAR, simulation->individual_soa.previous_states,
                     (i32) simulation->local_count, MPI_UNSIGNED_CHAR, 0, MPI_COMM_WORLD);
        MPI_Scatterv(NULL, NULL, NULL, MPI_UNSIGNED, simulation->individual_soa.times_in_state,
                     (i32) simulation->local_count, MPI_UNSIGNED, 0, MPI_COMM_WORLD);
        MPI_Scatterv(NULL, NULL, NULL, MPI_UNSIGNED, simulation->individual_soa.grid_poss_x,
                     (i32) simulation->local_count, MPI_UNSIGNED, 0, MPI_COMM_WORLD);
        MPI_Scatterv(NULL, NULL, NULL, MPI_UNSIGNED, simulation->individual_soa.grid_poss_y,
                     (i32) simulation->local_count, MPI_UNSIGNED, 0, MPI_COMM_WORLD);
        MPI_Scatterv(NULL, NULL, NULL, MPI_DOUBLE, simulation->individual_soa.exposeds, (i32) simulation->local_count,
                     MPI_DOUBLE, 0, MPI_COMM_WORLD);
        MPI_Scatterv(NULL, NULL, NULL, MPI_DOUBLE, simulation->individual_soa.infecteds, (i32) simulation->local_count,
                     MPI_DOUBLE, 0, MPI_COMM_WORLD);
        MPI_Scatterv(NULL, NULL, NULL, MPI_DOUBLE, simulation->individual_soa.recovereds, (i32) simulation->local_count,
                     MPI_DOUBLE, 0, MPI_COMM_WORLD);
    }

    MPI_Bcast(simulation->grid.cells, (i32) grid_cells, MPI_UNSIGNED_CHAR, 0, MPI_COMM_WORLD);
}

void simulation_step(Simulation *simulation)
{
    IndividualState *states = simulation->individual_soa.states;
    IndividualState *previous_states = simulation->individual_soa.previous_states;

    u32 *times_in_state = simulation->individual_soa.times_in_state;

    u32 *grid_poss_x = simulation->individual_soa.grid_poss_x;
    u32 *grid_poss_y = simulation->individual_soa.grid_poss_y;

    f64 *exposed_dur = simulation->individual_soa.exposeds;
    f64 *infected_dur = simulation->individual_soa.infecteds;
    f64 *recovered_dur = simulation->individual_soa.recovereds;

    u32 local_count = simulation->local_count;

    u32 local_data[4] = {0, 0, 0, 0};

    for (u32 local_individual_index = 0; local_individual_index < local_count; local_individual_index++)
    {
        switch (states[local_individual_index])
        {
            case Susceptible:
                local_data[0]++;
                const int num_inf_neighbs = simulation_get_num_infected_neighbors(
                    simulation, grid_poss_x[local_individual_index], grid_poss_y[local_individual_index]);
                if (should_transition(num_inf_neighbs))
                {
                    states[local_individual_index] = Exposed;
                    times_in_state[local_individual_index] = 0;
                }
                break;
            case Exposed:
                local_data[1]++;
                if (times_in_state[local_individual_index] > exposed_dur[local_individual_index])
                {
                    states[local_individual_index] = Infected;
                    times_in_state[local_individual_index] = 0;
                }
                break;
            case Infected:
                local_data[2]++;
                if (times_in_state[local_individual_index] > infected_dur[local_individual_index])
                {
                    states[local_individual_index] = Recovered;
                    times_in_state[local_individual_index] = 0;
                }
                break;
            case Recovered:
                local_data[3]++;
                if (times_in_state[local_individual_index] > recovered_dur[local_individual_index])
                {
                    states[local_individual_index] = Susceptible;
                    times_in_state[local_individual_index] = 0;
                }
                break;
        }
        times_in_state[local_individual_index]++;
    }

    u32 width = simulation->grid.width;
    u32 height = simulation->grid.height;

    memset(simulation->grid.cells, 0, width * height * sizeof(u8));

    for (u32 local_individual_index = 0; local_individual_index < simulation->local_count; local_individual_index++)
    {
        u32 new_x = rand_i32_uniform(0, width - 1);
        u32 new_y = rand_i32_uniform(0, height - 1);

        grid_poss_x[local_individual_index] = new_x;
        grid_poss_y[local_individual_index] = new_y;
        if (states[local_individual_index] == Infected)
        {
            simulation->grid.cells[new_y * width + new_x]++;
        }
    }

    MPI_Allreduce(MPI_IN_PLACE, simulation->grid.cells, (i32) width * height, MPI_UNSIGNED_CHAR, MPI_SUM,
                  MPI_COMM_WORLD);

    u32 global_counts[4];
    MPI_Allreduce(local_data, global_counts, 4, MPI_UNSIGNED, MPI_SUM, MPI_COMM_WORLD);
    OutputData *out = &simulation->output_data[simulation->current_iter];
    out->num_susceptible = global_counts[0];
    out->num_exposed = global_counts[1];
    out->num_infected = global_counts[2];
    out->num_recovered = global_counts[3];
}

#endif


void simulation_run(Simulation *simulation)
{
    for (simulation->current_iter = 0; simulation->current_iter < simulation->iterations_to_run; simulation->
         current_iter++)
    {
        simulation_step(simulation);
    }
}
