#include<stdio.h>
#include<mpi.h>
#include<unistd.h>

#define CHUNK_SIZE 1000
#define TAG_WORK 1
#define TAG_STOP 0

double f(double x){
    return 1.0/(1.0+x*x);
}

int main(int argc, char** argv){
    int rank, size;
    double start_time, end_time;
    long n = 1000000;
    double h = 1.0/n;
    double total_sum = 0.0;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    // Cette version nécessite au moins 2 processus.
    if(size < 2){
        if(rank == 0){
            printf("Ce programme nécessite au moins 2 processus.\n");
        }
        MPI_Finalize();
        return 1;
    }

    start_time = MPI_Wtime();

    if(rank == 0){
        // Processus maitre
        long next_index = 0;
        int active_workers = size - 1;
        double received_sum;
        MPI_Status status;

        for(int i = 1 ; i < size ; i++){
            MPI_Send(&next_index, 1, MPI_LONG, i, TAG_WORK, MPI_COMM_WORLD);
            next_index += CHUNK_SIZE;
        }

        while(active_workers > 0){
            MPI_Recv(&received_sum, 1, MPI_DOUBLE, MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
            total_sum += received_sum;

            if(next_index < n){
                MPI_Send(&next_index, 1, MPI_LONG, status.MPI_SOURCE, TAG_WORK, MPI_COMM_WORLD);
                next_index += CHUNK_SIZE;
            } else{
                MPI_Send(&next_index, 1, MPI_LONG, status.MPI_SOURCE, TAG_STOP, MPI_COMM_WORLD);
                active_workers--;
            }
        }
    } else{
        // Processus travailleur
        long start_index;
        MPI_Status status;

        while(1){
            MPI_Recv(&start_index, 1, MPI_LONG, 0, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
            if(status.MPI_TAG == TAG_STOP) break;

            double local_sum = 0.0;
            for(long i = start_index ; i < start_index + CHUNK_SIZE && i < n ; i++){
                double x = i * h;
                double val = (f(x) + f(x+h)) / 2.0;
                usleep((int)(val*1));
                local_sum += val;
            }

            MPI_Send(&local_sum, 1, MPI_DOUBLE, 0, 0, MPI_COMM_WORLD);
        }
    }

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