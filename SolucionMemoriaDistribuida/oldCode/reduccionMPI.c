#include <stdio.h>
#include <math.h>
#include <sys/time.h>
#include <stdint.h>
#include <stdlib.h>
#include <limits.h>
#include <mpi.h>

#define PRESICION 0.01

/** ***************************************************************************
 ****** Variables Globales al Programa pero Locales a la Maquina Actual. ******
 * *************************************************************************** */

int ID;     // ID de la Maquina Actual, autoasignada por MPI_Comm_rank
int nProcs; // Número de Maquinas Totales, autoasignada por MPI_Comm_size

int DIM = 4; // Tamaño de la Matriz - Asignacion estatica 2^randomNumber en caso de no ser pasado por parametro.

float *A; // Matriz A la cual sera enviada a los procesos
float *B; // Matriz B Resultado.

int from, to; // Rangos por los que se divide la matriz A

/** ******************************************************************************
 ****** Fin de Variables Globales al Programa pero Locales a la Maquina Actual. **
 * *************************************************************************** */

/** *******************************************************************
 * *************  DECLARACION DE PROCESOS   ***************************
 * ******************************************************************* */

void print_matrix_f(float *m)
{
    int i, j = 0;
    for (i = 0; i < DIM; i++)
    {
        printf("\n\t| ");
        for (j = 0; j < DIM; j++)
            printf("%2f ", m[i * DIM + j]);
        printf("|");
    }
    printf("\n");
}

/** *******************************************************************
 * *************  FIN DE DECLARACION DE PROCESOS   ********************
 * ******************************************************************* */

int main(int argc, char *argv[])
{

    int i, j, k;

    /** Init de MPI y obtencion del ID y el Numero de Procesos*/
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &ID);
    MPI_Comm_size(MPI_COMM_WORLD, &nProcs);

    /** Alocacion de memoria de las matrices*/
    A = (float *)malloc(sizeof(float *) * DIM * DIM);
    B = (float *)malloc(sizeof(float *) * DIM * DIM);

    /** Asigno el Rango que en el cual debo trabajar sobre la matriz.*/
    from = ID * (DIM / nProcs);
    to = from + (DIM / nProcs);

    /** El proceso con ID == 0 llena las matrices */
    if (ID == 0)
    {
        for (int i = 0; i < DIM; i++)
        {
            for (int j = 0; j < DIM; j++)
            {
                A[i * DIM + j] = rand() / (float)RAND_MAX; // Valores entre 0 y 1
                B[i * DIM + j] = 99;                       // Valor de Testeo para DEBUG
            }
        }

        // Print Matrix A
        printf("\n\tMatriz A\n");
        print_matrix_f(A);

        // Print Matrix B
        printf("\n\tMatriz B\n");
        print_matrix_f(B);
    }

    // Realizo el Broadcast de la matriz A (La Envio completa)
    MPI_Bcast(A, DIM * DIM, MPI_INT, 0, MPI_COMM_WORLD);

    free(A);
    free(B);

    MPI_Finalize();
    return 0;
}