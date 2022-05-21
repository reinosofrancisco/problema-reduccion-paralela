#include <stdio.h>
#include <math.h>
#include <sys/time.h>
#include <stdint.h>
#include <stdlib.h>
#include <limits.h>
#include <mpi.h>

void funcionThreadMaster(int, int , int);
void funcionThreads(int, int, int);

int PRESICION = 0.01;

int main(int argc, char *argv[]) {
    int id, nroProcesos, DIM;
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD,&id);
    MPI_Comm_size(MPI_COMM_WORLD,&nroProcesos);

    printf("Hilo id: %d \n", id);


    if (id == 0) {
        funcionThreadMaster(nroProcesos, DIM, id);
    } else {
        funcionThreads(id, DIM, nroProcesos);
    }

    MPI_Finalize();
    return(0);

}


void funcionThreadMaster(int nroProcesos, int N, int id) {

    //Aloco espacio para el arreglo y lo completo
    float* A = (float *)malloc(sizeof(float) * N);
    float* resultado = (float *)malloc(sizeof(float) * N);
    
    for (int i = 0; i < N; i++)
    {
        A[i] = rand() / (float) RAND_MAX;
    }

    int convergencia = 1; 

    do { 

        // Reparto el arreglo entre los distintos procesos
        for (int i = 1; i < nroProcesos; i++) {
            if (i == nroProcesos - 1) {
                MPI_Send(&(A[(i*(N/nroProcesos)) - 1]), (N/nroProcesos) + 1 , MPI_FLOAT, i, 99, MPI_COMM_WORLD);
            } else {
                MPI_Send(&(A[(i*(N/nroProcesos)) - 1]), (N/nroProcesos) + 2 , MPI_FLOAT, i, 99, MPI_COMM_WORLD);
            }
       
        }

        for (int i = 0; i < (N/nroProcesos); i++) {
            if (i == 0) {
                resultado[i] = (A[i] + A[i+1]) * 0.5;
            } else {
                resultado[i] = (A[i - 1] + A[i] + A[i + 1]) * 0.333333;
            }
        }

        for (int i = 1; i < nroProcesos; i++) {    
            MPI_Recv(&resultado[i * (N/nroProcesos)], (N/nroProcesos), MPI_FLOAT, i, 98, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        }

        convergencia = 1;

        for (int i = 0; i < N; i++) {
            convergencia = convergencia && (fabs(resultado[0] - resultado[i]) < PRESICION);
        }

        for (int i = 1; i < nroProcesos; i++) {
            MPI_Send(&convergencia, 1, MPI_INT, i, 97, MPI_COMM_WORLD);
        }

        for (int i = 0; i < N; i++) {
            A[i] = resultado[i];
        }

    } while (!convergencia);

    printf("Hilo con id: %d termino!! \n", id);

    free(A);
    free(resultado);
}

void funcionThreads(int id, int N, int nroProcesos) {

    int cantArray = id == nroProcesos - 1 ? (N/nroProcesos) + 1 : (N/nroProcesos) + 2;
    float* array = (float *)malloc(sizeof(float) * cantArray);
    float* resultado = (float *)malloc(sizeof(float) * ((N/nroProcesos)));
	
    printf("Slave id: %d\n", id);

    int convergencia = 1;
    
    do {

        MPI_Recv(array, cantArray, MPI_FLOAT, 0, 99, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        //Arranco en 1 porque como elemeto 0 le mando el ultimo valor del thread anterior
        for (int i = 1; i < cantArray - 1; i++) {
             if (i == (N/nroProcesos) - 1 && id == nroProcesos - 1) {
                resultado[i] = (array[i] + array[i - 1]) * 0.5;
            } else {
                resultado[i] = (array[i - 1] + array[i] + array[i + 1]) * 0.333333;
            }
        }

        MPI_Send(resultado, (N/nroProcesos), MPI_FLOAT, 0, 98, MPI_COMM_WORLD);

        MPI_Recv(&convergencia, 1, MPI_INT, 0, 97, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

    } while (!convergencia);

    printf("Hilo con id: %d termino!! \n", id);



    free(array);
    free(resultado);
}