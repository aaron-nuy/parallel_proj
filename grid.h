//
// Created by aaron on 17/03/2026.
//

#ifndef PARALLEL_PROJ_GRID_H
#define PARALLEL_PROJ_GRID_H
#include "individual.h"

typedef struct {
    Individual *individuals;
    u32 width;
    u32 height;
} Grid;

#endif //PARALLEL_PROJ_GRID_H