#include<stdio.h>
#include<mpi.h>
#include<unistd.h>

double f(double x){
    return 1.0/(1.0+x*x);
}

int main(int argc, char** argv){
    int rank, size;
    double start_time, end_time;
    long n = 1000000;
    double h = 1.0/n;
    double local_sum = 0.0, total_sum = 0.0;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    start_time = MPI_Wtime();

    // long local_n = n/size;
    // double local_a = rank*local_n*h;

    for(long i = rank ; i < n ; i += size){
        double x = i * h;
        double val = (f(x) + f(x+h)) / 2.0;

        usleep((int)(val*1));

        local_sum += val;
    }

    MPI_Reduce(&local_sum, &total_sum, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);

    MPI_Barrier(MPI_COMM_WORLD); // Synchronisation des processus
    end_time = MPI_Wtime();

    if(rank == 0){
        printf("==============================\n");
        printf("%ld itérations | %d processus\n", n, size);
        printf("Pi = %.15f\n", total_sum * h * 4.0);
        printf("Temps d'exécution : %.6f secondes\n", end_time - start_time);
        printf("==============================\n");
    }

    MPI_Finalize();
    return 0;
}