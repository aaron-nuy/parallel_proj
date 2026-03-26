#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <mpi.h>
#include <unistd.h>
#include <time.h>

#define PACKET_SIZE 10000000 // 10 millions
#define TAG_CONTINUE 1
#define TAG_STOP 0
#define TIME_LIMIT 10.0 // secondes

double rand01(){
    return rand() / (double)RAND_MAX;
}

int main(int argc, char** argv){
    int rank, size;

    MPI_Init(&argc, &argv);

    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    if(size < 2){
        if(rank == 0){
            printf("Ce programme nécessite au moins 2 processus.\n");
        }
        MPI_Finalize();

        return 1;
    }

    if(rank == 0){
        // processus serveur
        unsigned short clients = size - 1;
        unsigned long total_draws = 0;
        unsigned long N_total = 0;
        double elapsed_time, pi_final;

        int clients_running = clients;

        double t_start = MPI_Wtime();

        while(clients_running > 0){
            unsigned int N_in;
            MPI_Status status;

            MPI_Recv(&N_in, 1, MPI_INT, MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &status);

            N_total += N_in;
            total_draws += PACKET_SIZE;

            elapsed_time = MPI_Wtime() - t_start;

            if(elapsed_time < TIME_LIMIT){
                MPI_Send(NULL, 0, MPI_BYTE, status.MPI_SOURCE, TAG_CONTINUE, MPI_COMM_WORLD);
            } else{
                MPI_Send(NULL, 0, MPI_BYTE, status.MPI_SOURCE, TAG_STOP, MPI_COMM_WORLD);
                clients_running--;
            }

            pi_final = 4 * (double)N_total / (double)total_draws;
            elapsed_time = MPI_Wtime() - t_start;
        }
        printf("==============================\n");
        printf("Nombre de clients : %d\n", clients);
        printf("Pi estimé         : %.15f\n", pi_final);
        printf("Total tirages     : %lu\n", total_draws);
        printf("Paquets reçus     : %lu\n", N_total / PACKET_SIZE);
        printf("Temps écoulé      : %.6f secondes\n", elapsed_time);
        printf("==============================\n");
    } else{
        srand(rank + time(NULL)); // initialisation du générateur de nombres aléatoires

        while(1){
            unsigned int N_in = 0;

            for(unsigned int i = 0; i < PACKET_SIZE; i++){
                double x = rand01();
                double y = rand01();

                if(x*x + y*y <= 1.0){
                    N_in++;
                }
            }

            MPI_Send(&N_in, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);

            MPI_Status status;
            MPI_Recv(NULL, 0, MPI_BYTE, 0, MPI_ANY_TAG, MPI_COMM_WORLD, &status);

            if(status.MPI_TAG == TAG_STOP){
                break;
            }
        }
    }

    MPI_Finalize();

    return 0;
}