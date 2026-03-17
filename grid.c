//
// Created by aaron on 17/03/2026.
//

#include "grid.h"

#include <stdlib.h>


[[nodiscard]] static inline IntSet int_set_create(u32 min_capacity)
{
    return (IntSet)
    {
        .values = malloc(min_capacity * sizeof(u32)),
        .size = 0,
        .m_Capacity = min_capacity
    };
}

[[nodiscard]] static inline bool int_set_is_empty(IntSet *int_set)
{
    return int_set->size == 0;
}

[[nodiscard]] static inline u32 int_set_index_of_val(IntSet *int_set, u32 value)
{
    for (u32 i = 0; i < int_set->size; i++) {
        if (int_set->values[i] == value)
            return i;
    }
    return UINT32_MAX;
}


static inline void int_set_put(IntSet *int_set, u32 value)
{
    if (int_set->size >= int_set->m_Capacity)
    {
        int_set->m_Capacity += 2;
        int_set->values = realloc(int_set->values, int_set->m_Capacity * sizeof(u32));
    }
    int_set->values[int_set->size++] = value;
}

static inline void int_set_remove(IntSet *int_set, u32 value)
{
    u32 val_index = int_set_index_of_val(int_set, value);
    if (val_index != -1)
        int_set->values[val_index] = int_set->values[--int_set->size];
}

static inline void int_set_destroy(IntSet *int_set)
{
    free(int_set->values);
}

[[nodiscard]] static inline Grid grid_create(u32 width, u32 height)
{
    return (Grid)
    {
        .cells = malloc(width * height * sizeof(IntSet)),
        .width = width,
        .height = height
    };
}

static inline void grid_destroy(Grid *grid)
{
    u32 total = grid->width * grid->height;
    for (u32 i = 0; i < total; i++)
        int_set_destroy(&grid->cells[i]);
    free(grid->cells);
}

[[nodiscard]] static inline IntSet *grid_get_cell(Grid *grid, u32 x, u32 y)
{
    return &grid->cells[y * grid->width + x];
}
