cmake_minimum_required(VERSION 3.23)

project(parallel_proj C)

set(CMAKE_C_STANDARD 23)

set(CMAKE_C_FLAGS_RELEASE "-O3 -flto -DNDEBUG -mavx2")
set(CMAKE_C_FLAGS_DEBUG   "-O0 -g")

if (CMAKE_C_COMPILER_ID MATCHES "GNU")
    add_compile_options(-fno-diagnostics-color)
endif ()

find_package(MPI REQUIRED)

set(COMMON_SOURCES
        main.c
        rand_utils.c
        individual.c
        grid.c
        simulation.c
)

add_executable(seq_proj ${COMMON_SOURCES})
target_link_libraries(seq_proj PRIVATE m)
target_compile_definitions(seq_proj PRIVATE SEQUENTIAL)

add_executable(par_proj ${COMMON_SOURCES})
target_link_libraries(par_proj PRIVATE MPI::MPI_C m)
target_compile_definitions(par_proj PRIVATE PARALLEL)