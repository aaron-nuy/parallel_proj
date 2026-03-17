cmake_minimum_required(VERSION 3.23)

project(parallel_proj C)

set(CMAKE_C_STANDARD 23)
set(CMAKE_C_COMPILER mpicc)

set(CMAKE_C_FLAGS_RELEASE "-O3 -DNDEBUG")
set(CMAKE_C_FLAGS_DEBUG   "-O0 -g")

if(CMAKE_C_COMPILER_ID MATCHES "GNU")
    add_compile_options(-fno-diagnostics-color)
endif()

find_package(MPI REQUIRED)
find_package(raylib QUIET)
if(NOT raylib_FOUND)
    include(FetchContent)
    FetchContent_Declare(
            raylib
            GIT_REPOSITORY https://github.com/raysan5/raylib.git
            GIT_TAG        master
    )
    FetchContent_MakeAvailable(raylib)
endif()

add_executable(parallel_proj main.c
        typedefs.h
        rand_utils.h
        rand_utils.c
        individual.h
        grid.h)
target_link_libraries(parallel_proj PRIVATE MPI::MPI_C raylib m)