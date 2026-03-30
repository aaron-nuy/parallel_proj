//
// Created by aaron on 17/03/2026.
//

#include "grid.h"

#include <stdlib.h>
#include <string.h>

[[nodiscard]] Grid grid_create(u32 width, u32 height)
{
    u8* cells = malloc(width * height * sizeof(u8));

    memset(cells, 0, width * height * sizeof(u8));

    return (Grid)
    {
        .cells = cells,
        .width = width,
        .height = height
    };
}

void grid_add_individual(Grid *grid, u32 x, u32 y)
{
        u8 *cell = grid_get_cell(grid, x, y);
        (*cell)++;
}

void grid_remove_individual(Grid *grid, u32 x, u32 y)
{
        u8 *cell = grid_get_cell(grid, x, y);
        (*cell)--;
}

void grid_destroy(Grid *grid)
{
    free(grid->cells);
}

[[nodiscard]] u8* grid_get_cell(const Grid *grid, const u32 x, const u32 y)
{
    return &grid->cells[y * grid->width + x];
}

[[nodiscard]] u8 grid_get_cell_value(const Grid *grid, const u32 x, const u32 y)
{
    return grid->cells[y * grid->width + x];
}