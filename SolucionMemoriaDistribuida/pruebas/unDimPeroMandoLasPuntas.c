#include <stdio.h>
#include <math.h>
#include <sys/time.h>
#include <stdint.h>
#include <stdlib.h>
#include <limits.h>
#include <mpi.h>
#include <stdbool.h>

#define PRESICION 0.01

/** ***************************************************************************
 ****** Variables Globales al Programa pero Locales a la Maquina Actual. ******
 * *************************************************************************** */

MPI_Status status;

int ID;     // ID de la Maquina Actual, autoasignada por MPI_Comm_rank
int nProcs; // Número de Maquinas Totales, autoasignada por MPI_Comm_size

int DIM = 8; // Tamaño del Vector

float *A; // Vector A la cual sera enviada a los procesos
float *B; // Vector B Resultado.

int from, to; // Rangos por los que se divide el Vector A

/** ******************************************************************************
 ****** Fin de Variables Globales al Programa pero Locales a la Maquina Actual. **
 * *************************************************************************** */

/** *******************************************************************
 * *************  DECLARACION DE PROCESOS   ***************************
 * ******************************************************************* */

void print_vector(float *m)
{
    int i, j = 0;
    for (i = 0; i < DIM; i++)
    {
        printf("\n\t| ");
        printf("%2f ", m[i]);
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
    int dest, source;
    int offset = 0;
    bool convergencia;

    /** Init de MPI y obtencion del ID y el Numero de Procesos*/
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &ID);
    MPI_Comm_size(MPI_COMM_WORLD, &nProcs);

    /** Numero de Proceos Esclavos sin contar el ID 0. */
    int slaveTaskCount = nProcs - 1;
    /** Pedazo del vector que le corresponde a cada hijo. */
    int slaveSize = DIM / nProcs;

    /** Alocacion de memoria de los vectores*/
    A = (float *)malloc(sizeof(float *) * DIM);
    B = (float *)malloc(sizeof(float *) * DIM);

    /** COMPORTAMIENTO PROCESO PADRE */
    if (ID == 0)
    {
        /** Relleno el Vector A con valores entre 0 y 1 */
        for (int i = 0; i < DIM; i++)
        {
            A[i] = rand() / (float)RAND_MAX;
        }

        /** Message Tag 1 para el envio del Vector y el Offset.
         * Offset en 1 para saltearme las puntas.
         */
        /** SOLO ENVIO UNA VEZ LA MATRIZ */
        offset = slaveSize;
        for (dest = 1; dest <= slaveTaskCount; dest++)
        {
            // Envio el OFFSET del proceso esclavo.
            MPI_Send(&offset, 1, MPI_INT, dest, 1, MPI_COMM_WORLD);
            // Envio el Vector desde la posicion (OFFSET - 1) para que pueda calcular el vector reducido.
            MPI_Send(&A[offset - 1], slaveSize + 2, MPI_FLOAT, dest, 1, MPI_COMM_WORLD);

            // Modifico el Offset salteando por chunks de datos
            offset += slaveSize;
        }

        /** Mientras B no converga, envio A a los procesos y calculo */
        do
        {

            /** El Root calcula el primer chunk de datos */
            for (i = 0; i < slaveSize; i++)
            {
                if (i == 0)
                {
                    B[i] = ((A[i] + A[i + 1]) * (0.5));
                }
                else
                {
                    B[i] = (A[i - 1] + A[i] + A[i + 1]) * (0.333333);
                }
            }

            // el Root espera a que todos los procesos terminen. El message tag es 2
            for (int i = 1; i <= slaveTaskCount; i++)
            {
                source = i;
                /** Recibo el Offset y la parte de C que calculo el proceso esclavo*/
                MPI_Recv(&offset, 1, MPI_INT, source, 2, MPI_COMM_WORLD, &status);
                MPI_Recv(&B[offset], slaveSize, MPI_FLOAT, source, 2, MPI_COMM_WORLD, &status);
            }

            /** Parte II - Verificacion de Convergencia. */

            /** ENVIO SOLO LAS PUNTAS PARA VERIFICAR CONVERGENCIA */
            offset = slaveSize;
            for (dest = 1; dest <= slaveTaskCount; dest++)
            {
                // ENVIO LA POS OFFSET - 1 Y OFFSET + 1, ES DECIR LAS PUNTAS
                MPI_Send(&A[offset - 1], 1, MPI_FLOAT, dest, 1, MPI_COMM_WORLD);
                MPI_Send(&A[offset + 1], 1, MPI_FLOAT, dest, 1, MPI_COMM_WORLD);

                // Modifico el Offset salteando por chunks de datos
                offset += slaveSize;
            }

            convergencia = true;

            for (i = 0; i < DIM; i++)
            {
                convergencia = convergencia && (fabs(B[0] - B[i]) < PRESICION);
            }

            if (!convergencia)
            {
                // Copio todo el Vector B en A, y vuelvo a utilizar B como auxiliar
                for (int i = 0; i < DIM; i++)
                {
                    A[i] = B[i];
                }
            }

            offset = slaveSize;
            for (dest = 1; dest <= slaveTaskCount; dest++)
            {
                // Envio el OFFSET del proceso esclavo.
                MPI_Send(&offset, 1, MPI_INT, dest, 1, MPI_COMM_WORLD);
                // Envio el valor de Convergencia
                MPI_Send(&convergencia, 1, MPI_INT, dest, 3, MPI_COMM_WORLD);
                // Modifico el Offset salteando por chunks de datos
                offset += slaveSize;
            }

        } while (!convergencia);
        print_vector(B);
    }

    /** COMPORTAMIENTO PROCESOS HIJOS */
    if (ID > 0)
    {

        /** SOLO RECIBO UNA VEZ LA MATRIZ */

        // El Source sera siempre el root (Master) con ID = 0
        source = 0;
        /** El esclavo espera por el mensaje con tag 1 que envia el root
         * Recibo el offset, pedazo de A y la totalidad de la Matriz B.*/
        MPI_Recv(&offset, 1, MPI_INT, source, 1, MPI_COMM_WORLD, &status);
        MPI_Recv(&A[offset - 1], slaveSize + 2, MPI_FLOAT, source, 1, MPI_COMM_WORLD, &status);

        do
        {

            for (i = offset; i < offset + slaveSize; i++)
            {
                if (i == DIM - 1)
                {
                    B[i] = ((A[i - 1] + A[i]) * (0.5));
                }
                else
                {
                    B[i] = (A[i - 1] + A[i] + A[i + 1]) * (0.333333);
                }
            }

            /** Envio el Offset y Resultado B con Message Tag = 2*/
            MPI_Send(&offset, 1, MPI_INT, 0, 2, MPI_COMM_WORLD);
            MPI_Send(&B[offset], slaveSize, MPI_FLOAT, source, 2, MPI_COMM_WORLD);

            /** RECIBO LAS PUNTAS PARA VERIFICAR CONVERGENCIA */
            MPI_Recv(&A[offset - 1], 1, MPI_INT, source, 1, MPI_COMM_WORLD, &status);
            MPI_Recv(&A[offset + 1], 1, MPI_FLOAT, source, 1, MPI_COMM_WORLD, &status);

            /** Mensaje para verificar convergencia con Message Tag = 3*/
            MPI_Recv(&offset, 1, MPI_INT, source, 1, MPI_COMM_WORLD, &status);
            MPI_Recv(&convergencia, 1, MPI_INT, source, 3, MPI_COMM_WORLD, &status);

            if (!convergencia)
            {
                // Copio todo el Vector B en A, y vuelvo a utilizar B como auxiliar
                for (i = offset; i < offset + slaveSize; i++)
                {
                    A[i] = B[i];
                }
            }

            if (ID == 2)
            {
                print_vector(B);
                sleep(3);
            }

        } while (!convergencia);
    }

    free(A);
    free(B);

    MPI_Finalize();
    return 0;
}