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

MPI_Status status;

int ID;     // ID de la Maquina Actual, autoasignada por MPI_Comm_rank
int nProcs; // Número de Maquinas Totales, autoasignada por MPI_Comm_size

int DIM = 64; // Tamaño del Vector por defecto

float *A; // Vector A la cual sera enviada a los procesos
float *B; // Vector B Resultado.

int from, to; // Rangos por los que se divide el Vector A

/** ******************************************************************************
 ****** Fin de Variables Globales al Programa pero Locales a la Maquina Actual. **
 * *************************************************************************** */

/** *******************************************************************
 * *************  DECLARACION DE PROCESOS   ***************************
 * ******************************************************************* */

/** Funcion para comparar tiempos */
double dwalltime()
{
    double sec;
    struct timeval tv;

    gettimeofday(&tv, NULL);
    sec = tv.tv_sec + tv.tv_usec / 1000000.0;
    return sec;
}

/** *******************************************************************
 * *************  FIN DE DECLARACION DE PROCESOS   ********************
 * ******************************************************************* */

int main(int argc, char *argv[])
{

    int i, j, k;
    int dest, source;
    int offset = 0;
    int convergencia;
    int convergenciaLocal;

    /** Init de MPI y obtencion del ID y el Numero de Procesos*/
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &ID);
    MPI_Comm_size(MPI_COMM_WORLD, &nProcs);

    // Controla los argumentos al programa
    if ((argc != 2) || ((DIM = atoi(argv[1])) <= 0))
    {
        if (ID == 0)
        {
            printf("\n Compilar con: mpicc -o archivoCompilado archivoFuente.c");
            printf("\n Correr con: mpirun -np numThreads archivoCompilado DIM");
            printf("\n\n numThreads: Numero de threads a usar");
            printf("\n archivoCompilado: Archivo compilado a ejecutar");
            printf("\n DIM: Dimension del vector\n");
        }
        exit(1);
    }

    /** Numero de Proceos Esclavos sin contar el ID 0. */
    int slaveTaskCount = nProcs - 1;
    /** Pedazo del vector que le corresponde a cada hijo. */
    int slaveSize = DIM / nProcs;

    /** COMPORTAMIENTO PROCESO PADRE */
    if (ID == 0)
    {

        /** Alocacion de memoria de los vectores para el padre */
        A = (float *)malloc(sizeof(float) * DIM);
        B = (float *)malloc(sizeof(float) * DIM);

        /** Relleno el Vector A con valores entre 0 y 1 */
        for (int i = 0; i < DIM; i++)
        {
            A[i] = rand() / (float)RAND_MAX;
        }

        printf("Vector Original de size %d\n", DIM);

        /* Inicio de la medicion de tiempo */
        double timetick;
        timetick = dwalltime();

        /** Mientras B no converga, envio A a los procesos y calculo */
        do
        {
            /** Parte I - Reduccion. */

            /** Message Tag 1 para el envio del Vector */
            offset = slaveSize;
            for (dest = 1; dest <= slaveTaskCount; dest++)
            {
                /* Envio el Vector A desde OFFSET - 1 para incluir puntas*/
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

            for (int i = 1; i <= slaveTaskCount; i++)
            {
                MPI_Send(&B[0], 1, MPI_FLOAT, i, 2, MPI_COMM_WORLD);
            }

            /** Verificacion de convergencia */

            /** Parte II - Verificacion de Convergencia. */

            convergenciaLocal = 1;

            // El Root verifica la convergencia del primer chunk de datos.
            for (i = 0; i < slaveSize; i++)
            {
                convergenciaLocal = convergenciaLocal && (fabs(B[0] - B[i]) < PRESICION);
            }

            MPI_Allreduce(&convergenciaLocal, &convergencia, 1, MPI_INT, MPI_LAND, MPI_COMM_WORLD);

            // Si no converge, el root copia todos los valores de B a A.
            if (!convergencia)
            {
                // Copio todo el Vector B en A, y vuelvo a utilizar B como auxiliar
                for (int i = 0; i < DIM / nProcs; i++)
                {
                    A[i] = B[i];
                }
            }

            for (i = 1; i < nProcs; i++) {
                if (convergencia) {
                    MPI_Recv(&B[i * (DIM/nProcs)], DIM/nProcs, MPI_FLOAT, i, 2, MPI_COMM_WORLD, &status);
                } else {
                    MPI_Recv(&A[i * (DIM/nProcs)], DIM/nProcs, MPI_FLOAT, i, 2, MPI_COMM_WORLD, &status);
                }
            }


        } while (!convergencia);

        /* Fin de la medicion de tiempo */
        printf("Tiempo en segundos para convergencia %f manejado por el hijo ID = %d\n", (dwalltime() - timetick), ID);
    }

    /** COMPORTAMIENTO PROCESOS HIJOS */
    if (ID > 0)
    {

        /** Alocacion de memoria de los vectores para los chunks de datos de los hijos.
         * Esto es, slaveSize + las puntas.
         * Los datos de los hijos son A[1] hasta A[slaveSize]
         * Las puntas son A[0] y A[slaveSize + 1]
         */
        A = (float *)malloc(sizeof(float) * (slaveSize + 2));
        /* En B[0] comienza lo que calcula cada proceso hijo. */
        B = (float *)malloc(sizeof(float) * (slaveSize));

        /* Variables auxiliares */
        float b_cero_root;

        /* Inicio de la medicion de tiempo para el HIJO ID > 0 */
        double timetick;
        timetick = dwalltime();

        do
        {
            /** Parte I - Reduccion. */

            // El Source sera siempre el root (Master) con ID = 0
            source = 0;
            /** El esclavo espera por el mensaje con tag 1 que envia el root
             * Recibo entonces el pedazo de A que me corresponde + Las 2 puntas. */
            MPI_Recv(&A[0], slaveSize + 2, MPI_FLOAT, source, 1, MPI_COMM_WORLD, &status);

            /** Calculo para mis datos, desde i = 1 hasta SlaveSize inclusive. */
            for (i = 1; i < slaveSize + 1; i++)
            {
                /* Si soy el ultimo proceso y estoy en el ultimo dato*/
                if ((ID == slaveTaskCount) && (i == slaveSize))
                {
                    B[i - 1] = ((A[i - 1] + A[i]) * (0.5));
                }
                /* Comportamiento normal. */
                else
                {
                    B[i - 1] = (A[i - 1] + A[i] + A[i + 1]) * (0.333333);
                }
            }

            /** Recibo B[0] en una variable auxiliar para calcular mi propia convergencia.*/
            MPI_Recv(&b_cero_root, 1, MPI_FLOAT, source, 2, MPI_COMM_WORLD, &status);

            /** Parte II - Verificacion de Convergencia. */

            convergenciaLocal = 1;

            for (i = 0; i < slaveSize; i++)
            {
                convergenciaLocal = convergenciaLocal && (fabs(b_cero_root - B[i]) < PRESICION);
            }
            
            MPI_Allreduce(&convergenciaLocal, &convergencia, 1, MPI_INT, MPI_LAND, MPI_COMM_WORLD);

            /** Envio el resultado B con Message Tag = 2. */
            MPI_Send(&B[0], slaveSize, MPI_FLOAT, source, 2, MPI_COMM_WORLD);

        } while (!convergencia);

        /* Fin de la medicion de tiempo */
        printf("Tiempo en segundos para convergencia %f para el hijo ID = %d\n", (dwalltime() - timetick), ID);
    }

    free(A);
    free(B);

    MPI_Finalize();
    return 0;
}