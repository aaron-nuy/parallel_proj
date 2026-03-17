//
// Created by aaron on 17/03/2026.
//

#ifndef PARALLEL_PROJ_GRID_H
#define PARALLEL_PROJ_GRID_H
#include "individual.h"

const u32 MIN_INT_SET_CAPACITY = 5;

typedef struct {
    u32 *values;
    u32 size;
    u32 m_Capacity;

    // TODO: maybe cache number of infected individuals in cell to a field?
} IntSet;

[[nodiscard]] static inline IntSet int_set_create(u32 min_capacity);

[[nodiscard]] static inline bool int_set_is_empty(IntSet *int_set);

[[nodiscard]] static inline u32 int_set_index_of_val(IntSet *int_set, u32 value);

static inline void int_set_put(IntSet *int_set, u32 value);

static inline void int_set_remove(IntSet *int_set, u32 value);

static inline void int_set_destroy(IntSet *int_set);

typedef struct {
    IntSet *cells;
    u32 width;
    u32 height;
} Grid;

[[nodiscard]] static inline Grid grid_create(u32 width, u32 height);

[[nodiscard]] static inline IntSet* grid_get_cell(Grid *grid, u32 x, u32 y);

static inline void grid_destroy(Grid *grid);


#endif //PARALLEL_PROJ_GRID_H
