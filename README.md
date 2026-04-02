## Simulation SEIR

### Instructions de compilation

Nécessite une version de CMake supérieure ou égale à 3.20 et MPI pour compiler le projet

```bash
mkdir -p build && cd build
cmake .. -DCMAKE_BUILD_TYPE=<Release|Debug> # choisissez l’un des deux
cmake --build . --target seq_proj
cmake --build . --target par_proj
```

### Exécution

Pour `seq_proj`:
```./build/seq_proj```

Pour `par_proj`:
```mpiexec -n <NOMBRE_DE_PROCESSUS> ./build/par_proj```


## PI_MPI_det
```bash
cd PI_MPI_det
make
```

## PI_MPI_sto

### Instructions de compilation

```bash
cd PI_MPI_sto
make
```

### Exécution
```make run N=<NOMBRE_DE_FOIS_A_EXECUTER>```
