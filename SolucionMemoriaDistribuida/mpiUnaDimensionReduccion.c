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
    bool convergenciaLocal[DIM];

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

        /** Mientras B no converga, envio A a los procesos y calculo */
        do
        {
            /** Message Tag 1 para el envio del Vector y el Offset.
             * Offset en 1 para saltearme las puntas.
             */
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

                /** Envio B[0] para que cada hilo calcule su propia convergencia. */
                MPI_Send(&B[0], 1, MPI_FLOAT, source, 2, MPI_COMM_WORLD);

                /** Envio las puntas para no enviar todo el vector nuevamente. */
                // MPI_Send(&B[offset - 1], 1, MPI_FLOAT, source, 2, MPI_COMM_WORLD);
                // MPI_Send(&B[offset + 1], 1, MPI_FLOAT, source, 2, MPI_COMM_WORLD);
            }

            /** Verificacion de convergencia */

            /** Parte II - Verificacion de Convergencia. */

            convergencia = true;

            // El Root verifica la convergencia del primer chunk de datos.
            for (i = 0; i < slaveSize; i++)
            {
                convergencia = convergencia && (fabs(B[0] - B[i]) < PRESICION);
            }

            // El root recibe la respuesta de los procesos esclavos
            for (int i = 1; i <= slaveTaskCount; i++)
            {
                source = i;
                MPI_Recv(&convergenciaLocal[i], 1, MPI_INT, source, 3, MPI_COMM_WORLD, &status);
                convergencia = convergencia && convergenciaLocal[i];
            }


            // Si no converge, el root copia todos los valores de B a A.
            if (!convergencia)
            {
                // Copio todo el Vector B en A, y vuelvo a utilizar B como auxiliar
                for (int i = 0; i < DIM; i++)
                {
                    A[i] = B[i];
                }
            }

            for (dest = 1; dest <= slaveTaskCount; dest++)
            {
                // Envio el valor de Convergencia a todos los hijos para que sepan si seguir o no reduciendo.
                MPI_Send(&convergencia, 1, MPI_INT, dest, 3, MPI_COMM_WORLD);
            }

        } while (!convergencia);
        
        print_vector(B);
    }

    /** COMPORTAMIENTO PROCESOS HIJOS */
    if (ID > 0)
    {
        do
        {
            // El Source sera siempre el root (Master) con ID = 0
            source = 0;
            /** El esclavo espera por el mensaje con tag 1 que envia el root
             * Recibo el offset, pedazo de A y la totalidad de la Matriz B.*/
            MPI_Recv(&offset, 1, MPI_INT, source, 1, MPI_COMM_WORLD, &status);
            MPI_Recv(&A[offset - 1], slaveSize + 2, MPI_FLOAT, source, 1, MPI_COMM_WORLD, &status);

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

            /** Recibo B[0] para calcular mi propia convergencia. */
            MPI_Recv(&B[0], 1, MPI_FLOAT, source, 2, MPI_COMM_WORLD, &status);

            /** Recibo las puntas para no tener que recibir todo el vector nuevamente. */
            // MPI_Recv(&B[offset - 1], 1, MPI_FLOAT, source, 2, MPI_COMM_WORLD, &status);
            // MPI_Recv(&B[offset + 1], 1, MPI_FLOAT, source, 2, MPI_COMM_WORLD, &status);

            // Verifico mi propia convergencia
            convergencia = true;
            for (i = offset; i < offset + slaveSize; i++)
            {
                convergencia = convergencia && (fabs(B[0] - B[i]) < PRESICION);
            }

            // Envio mi convergencia como proceso Hijo para que el Padre compare con las demas.
            MPI_Send(&convergencia, 1, MPI_INT, source, 3, MPI_COMM_WORLD);

            /** Mensaje para verificar convergencia con Message Tag = 3.
             * Recibo del padre si todos los procesos convergen. Caso contrario, vuelvo a calcular.
             */
            MPI_Recv(&convergencia, 1, MPI_INT, source, 3, MPI_COMM_WORLD, &status);

        } while (!convergencia);
    }

    free(A);
    free(B);

    MPI_Finalize();
    return 0;
}