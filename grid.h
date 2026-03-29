//
// Created by aaron on 17/03/2026.
//

#ifndef PARALLEL_PROJ_GRID_H
#define PARALLEL_PROJ_GRID_H
#include "typedefs.h"

typedef struct {
    u8* cells;
    u32 width;
    u32 height;
} Grid;

[[nodiscard]] Grid grid_create(u32 width, u32 height);

[[nodiscard]] u8 *grid_get_cell(const Grid *grid, u32 x, u32 y);

[[nodiscard]] u8 grid_get_cell_value(const Grid *grid, u32 x, u32 y);

void grid_add_individual(Grid *grid, u32 x, u32 y, bool infected);

void grid_remove_individual(Grid *grid, u32 x, u32 y, bool infected);

void grid_destroy(Grid *grid);


#endif //PARALLEL_PROJ_GRID_H
