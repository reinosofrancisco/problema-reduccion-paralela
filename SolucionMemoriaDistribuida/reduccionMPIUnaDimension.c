#include <stdio.h>
#include <stdbool.h>
#include <math.h>
#include <sys/time.h>
#include <stdint.h>
#include <stdlib.h>
#include <limits.h>
#include <mpi.h>

void funcionThreadMaster(int, int , int);
void funcionThreads(int, int, int);



int main(int argc, char *argv[]) {
    int id, nroProcesos, DIM;
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD,&miID);
    MPI_Comm_size(MPI_COMM_WORLD,&nrProcesos);


    if (id = 0) {
        funcionThreadMaster(nroProcesos, DIM, id);
    } else {
        funcionThreads(id, N, nroProcesos);
    }

    MPI_Finalize();
    return(0);

}


void funcionThreadMaster(int nroProcesos, int N, int id) {

    //Aloco espacio para el arreglo y lo completo
    float* A = (float *)malloc(sizeof(float) * N);
    float* resultado = (float *)malloc(sizeof(float) * ((N/nroProcesos)))
    
    for (int i = 0; i < N; i++)
    {
        A[i] = rand() / (float) RAND_MAX;
    }

    bool convergencia; 
    
    // Reparto el arreglo entre los distintos procesos
    for (int i = 1; i < N; i++) {
        if (i != N-1) {
            MPI_Send(&(A[(i*id)-1]), (N/nroProcesos) + 2 , MPI_FLOAT, i, 99, MPI_COMM_WORLD);
        } else {
            MPI_Send(&(A[(i*id)-1]), (N/nroProcesos) + 1 , MPI_FLOAT, i, 99, MPI_COMM_WORLD);
        }
       
    }

    /*

    do { 
        for (int i = 0; i < (N/nroProcesos) + 2; i++) {
            if (i == 0) {
                resultado[i] = (A[i] + A[i+1]) * 0.5;
            } else {
                resultado[i] = (A[i - 1] + A[i] + A[i + 1]) * 0.333333;
            }
        }
        MPI_Barrier(MPI_COMM_WORLD);

        for (i = 1; i < N; i++) {
            MPI_Send(resultado[0], 1, MPI_FLOAT, i, 98, MPI_COMM_WORLD);
        } 
            

        convergencia = true;



    } while (!convergencia)

    */

    free(A);
    free(resultado);
}

void funcionThreads(int id, int N, int nroProcesos) {

    int cantArray = id == 3 ? (N/nroProcesos) + 1 : (N/nroProcesos) + 2;
    float* array = (float *)malloc(sizeof(float) * cantArray);
    float* resultado = (float *)malloc(sizeof(float) * ((N/nroProcesos)))

    MPI_Recv(array, cantArray, MPI_FLOAT, 0, 99, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
	
    printf("Slave id: %d\n", id);


    free(array);
    free(resultado);
}