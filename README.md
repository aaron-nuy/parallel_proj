# parallel_proj - Instruction de Build

## Simulation SEIR

Nécessite une version de CMake supérieure ou égale à 3.20 et MPI pour compiler le projet

```bash
mkdir -p build && cd build
cmake .. -DCMAKE_BUILD_TYPE=<Release|Debug> # choisissez l’un des deux
cmake --build . --target seq_proj
cmake --build . --target par_proj
```

## PI_MPI_det
```bash
cd PI_MPI_det
make
```

## PI_MPI_sto
```bash
cd PI_MPI_sto
make
```
