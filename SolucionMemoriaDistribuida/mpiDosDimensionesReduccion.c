#include <stdio.h>
#include <math.h>
#include <sys/time.h>
#include <stdint.h>
#include <stdlib.h>
#include <limits.h>
#include <mpi.h>

#define PRESICION 0.01

#define true 1
#define false 0
#define bool int

/** ***************************************************************************
 ****** Variables Globales al Programa pero Locales a la Maquina Actual. ******
 * *************************************************************************** */

MPI_Status status;

int ID;     // ID de la Maquina Actual, autoasignada por MPI_Comm_rank
int nProcs; // Número de Maquinas Totales, autoasignada por MPI_Comm_size

int DIM = 64; // Tamaño de la matriz por defecto

float *A; // matriz A la cual sera enviada a los procesos
float *B; // matriz B Resultado.

int from, to; // Rangos por los que se divide el matriz A

int slaveSize;

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
            printf("\n DIM: Dimension de la matriz (DIM * DIM)\n");
        }
        exit(1);
    }

    /** Numero de Proceos Esclavos sin contar el ID 0. */
    int slaveTaskCount = nProcs - 1;
    /** Pedazo del vector que le corresponde a cada hijo. */
    slaveSize = (DIM * DIM) / nProcs;

    int convergencia;
    int convergenciaLocal;

    int offset = 0;
    register float aux; // Register for multiple accesses to the same variable

    /** COMPORTAMIENTO PROCESO PADRE */
    if (ID == 0)
    {

        /** Alocacion de memoria de la matriz para el padre */
        A = (float *)malloc(sizeof(float) * DIM * DIM);
        B = (float *)malloc(sizeof(float) * DIM * DIM);

        /** Relleno el Vector A con valores entre 0 y 1 */
        for (int i = 0; i < DIM; i++)
        {
            for (int j = 0; j < DIM; j++)
            {
                A[i * DIM + j] = rand() / (float)RAND_MAX;
            }
        }

        printf("Matriz Original de size %dx%d\n", DIM, DIM);

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
                /* Envio el Vector A desde OFFSET - DIM para incluir las filas externas. */
                MPI_Send(&A[offset - DIM], slaveSize + (2 * DIM), MPI_FLOAT, dest, 1, MPI_COMM_WORLD);

                // Modifico el Offset salteando por chunks de datos
                offset += slaveSize;
            }

            /** El Root calcula el primer chunk de datos */
            for (i = 0; i < slaveSize / DIM; i++)
            {
                for (j = 0; j < DIM; j++)
                {
                    if (i == 0 && j == 0)
                        // Primer cubito Primer fila
                        B[i * DIM + j] = (A[i * DIM + j] + A[(i + 1) * DIM + j] + A[i * DIM + (j + 1)] + A[(i + 1) * DIM + (j + 1)]) * (0.25);
                    else if (i == 0 && j == DIM - 1)
                    {
                        // Ultimo cubito Primer fila
                        B[i * DIM + j] = (A[i * DIM + j] + A[(i + 1) * DIM + j] + A[i * DIM + (j - 1)] + A[(i + 1) * DIM + (j - 1)]) * (0.25);
                    }
                    else if (i == 0)
                    {
                        // Primer fila sin puntas
                        aux = 0;
                        aux += (A[i * DIM + j] + A[i * DIM + (j - 1)] + A[i * DIM + (j + 1)]);
                        aux += (A[(i + 1) * DIM + j] + A[(i + 1) * DIM + (j - 1)] + A[(i + 1) * DIM + (j + 1)]);
                        B[i * DIM + j] = (aux * 0.166666);
                    }
                    else if (j == 0)
                    {
                        // Primera columna sin puntas
                        aux = 0;
                        aux += (A[i * DIM + j] + A[(i - 1) * DIM + j] + A[(i + 1) * DIM + j]);
                        aux += (A[i * DIM + (j + 1)] + A[(i - 1) * DIM + (j + 1)] + A[(i + 1) * DIM + (j + 1)]);
                        B[i * DIM + j] = (aux * 0.166666);
                    }
                    else if (j == DIM - 1)
                    {
                        // Ultima columna sin puntas
                        aux = 0;
                        aux += (A[i * DIM + j] + A[(i - 1) * DIM + j] + A[(i + 1) * DIM + j]);
                        aux += (A[i * DIM + (j - 1)] + A[(i - 1) * DIM + (j - 1)] + A[(i + 1) * DIM + (j - 1)]);
                        B[i * DIM + j] = (aux * 0.166666);
                    }
                    else
                    {
                        // Caso comun
                        aux = 0;
                        aux += (A[i * DIM + j] + A[i * DIM + (j - 1)] + A[i * DIM + (j + 1)]);
                        aux += (A[(i - 1) * DIM + j] + A[(i - 1) * DIM + (j - 1)] + A[(i - 1) * DIM + (j + 1)]);
                        aux += (A[(i + 1) * DIM + j] + A[(i + 1) * DIM + (j - 1)] + A[(i + 1) * DIM + (j + 1)]);
                        B[i * DIM + j] = (aux * 0.111111);
                    }
                }
            }

            // el Root espera a que todos los procesos terminen. El message tag es 2
            offset = slaveSize;
            for (int i = 1; i <= slaveTaskCount; i++)
            {
                source = i;
                /** Recibo la parte de B que calculo el Slave. No recibo offset. */
                MPI_Recv(&B[offset], slaveSize, MPI_FLOAT, source, 2, MPI_COMM_WORLD, &status);

                /** Envio B[0] para que cada hilo calcule su propia convergencia. */
                MPI_Send(&B[0], 1, MPI_FLOAT, source, 2, MPI_COMM_WORLD);

                offset += slaveSize;
            }

            /** Parte II - Verificacion de Convergencia. */
            convergenciaLocal = 1;
            // El Root verifica la convergencia del primer chunk de datos.
            for (i = 0; i < (slaveSize / DIM); i++)
            {
                for (j = 0; j < DIM; j++)
                {
                    convergenciaLocal = convergenciaLocal && (fabs(B[0] - B[i * DIM + j]) < PRESICION);
                }
            }

            MPI_Reduce(&convergenciaLocal, &convergencia, 1, MPI_INT, MPI_LAND, 0, MPI_COMM_WORLD);


            // Si no converge, el root copia todos los valores de B a A.
            if (!convergencia)
            {
                // Copio toda la matriz B en A, y vuelvo a utilizar B como auxiliar
                for (i = 0; i < DIM; i++)
                {
                    for (j = 0; j < DIM; j++)
                    {
                        A[i * DIM + j] = B[i * DIM + j];
                    }
                }
            }

            for (dest = 1; dest <= slaveTaskCount; dest++)
            {
                // Envio el valor de Convergencia a todos los hijos para que sepan si seguir o no reduciendo.
                MPI_Send(&convergencia, 1, MPI_INT, dest, 3, MPI_COMM_WORLD);
            }

        } while (!convergencia);

        /* Fin de la medicion de tiempo del padre*/
        printf("Tiempo en segundos para convergencia %f para el root ID = %d\n", (dwalltime() - timetick), ID);
    }

    /** COMPORTAMIENTO PROCESOS HIJOS */
    if (ID > 0)
    {

        /** Alocacion de memoria de los vectores para los chunks de datos de los hijos.
         * Esto es, slaveSize + las puntas.
         * Los datos de los hijos son A[DIM] hasta A[DIM + slaveSize - 1]
         * La primer fila es A[0] hasta A[DIM - 1]
         * La ultima fila es A[DIM + slaveSize] hata A[DIM + slaveSize + DIM - 1]
         */
        A = (float *)malloc(sizeof(float) * (slaveSize + (2 * DIM)));
        /* en B[0] comienza lo que calcula cada hilo para B. */
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
             * Recibo entonces el pedazo de A que me corresponde y
             * la fila superior e inferior al chunk que me corresponde. */
            MPI_Recv(&A[0], slaveSize + (2 * DIM), MPI_FLOAT, source, 1, MPI_COMM_WORLD, &status);

            /** Calculo para mis datos, desde i = 1 hasta (slaveSize/DIM) + 1. */
            for (i = 1; i < (slaveSize / DIM) + 1; i++)
            {
                for (j = 0; j < DIM; j++)
                {

                    if ((ID == slaveTaskCount) && (i == (slaveSize / DIM)))
                    {

                        if (j == 0)
                        {
                            // Primer cubito Ultima fila
                            B[(i - 1) * DIM + j] = (A[i * DIM + j] + A[(i - 1) * DIM + j] + A[i * DIM + (j + 1)] + A[(i - 1) * DIM + (j + 1)]) * (0.25);
                        }
                        else if (j == DIM - 1)
                        {
                            // Ultimo cubito Ultima fila
                            B[(i - 1) * DIM + j] = (A[i * DIM + j] + A[(i - 1) * DIM + j] + A[i * DIM + (j - 1)] + A[(i - 1) * DIM + (j - 1)]) * (0.25);
                        }
                        else
                        {
                            // Ultima fila sin puntas
                            aux = 0;
                            aux += (A[i * DIM + j] + A[i * DIM + (j - 1)] + A[i * DIM + (j + 1)]);
                            aux += (A[(i - 1) * DIM + j] + A[(i - 1) * DIM + (j - 1)] + A[(i - 1) * DIM + (j + 1)]);
                            B[(i - 1) * DIM + j] = (aux * 0.166666);
                        }
                    }
                    else if (j == 0)
                    {
                        // Primera columna sin puntas
                        aux = 0;
                        aux += (A[i * DIM + j] + A[(i - 1) * DIM + j] + A[(i + 1) * DIM + j]);
                        aux += (A[i * DIM + (j + 1)] + A[(i - 1) * DIM + (j + 1)] + A[(i + 1) * DIM + (j + 1)]);
                        B[(i - 1) * DIM + j] = (aux * 0.166666);
                        // if (ID == 1 ) printf("B[i * DIM + j] = %f\n", B[i * DIM + j]);
                    }
                    else if (j == DIM - 1)
                    {
                        // Ultima columna sin puntas
                        aux = 0;
                        aux += (A[i * DIM + j] + A[(i - 1) * DIM + j] + A[(i + 1) * DIM + j]);
                        aux += (A[i * DIM + (j - 1)] + A[(i - 1) * DIM + (j - 1)] + A[(i + 1) * DIM + (j - 1)]);
                        B[(i - 1) * DIM + j] = (aux * 0.166666);
                    }
                    else
                    {
                        // Caso comun
                        aux = 0;
                        aux += (A[i * DIM + j] + A[i * DIM + (j - 1)] + A[i * DIM + (j + 1)]);
                        aux += (A[(i - 1) * DIM + j] + A[(i - 1) * DIM + (j - 1)] + A[(i - 1) * DIM + (j + 1)]);
                        aux += (A[(i + 1) * DIM + j] + A[(i + 1) * DIM + (j - 1)] + A[(i + 1) * DIM + (j + 1)]);
                        B[(i - 1) * DIM + j] = (aux * 0.111111);
                    }
                }
            }

            /** Envio el resultado B con Message Tag = 2. */
            MPI_Send(&B[0], slaveSize, MPI_FLOAT, source, 2, MPI_COMM_WORLD);

            /** Recibo B[0] en un auxiliar para calcular mi propia convergencia. */
            MPI_Recv(&b_cero_root, 1, MPI_FLOAT, source, 2, MPI_COMM_WORLD, &status);

            /** Parte II - Verificacion de Convergencia. */

            convergenciaLocal = 1;

            for (i = 0; i < (slaveSize / DIM); i++)
            {
                for (j = 0; j < DIM; j++)
                {
                    convergenciaLocal = convergenciaLocal && (fabs(b_cero_root - B[i * DIM + j]) < PRESICION);
                }
            }

            
            MPI_Reduce(&convergenciaLocal, &convergencia, 1, MPI_INT, MPI_LAND, 0, MPI_COMM_WORLD);

            /** Mensaje para verificar convergencia con Message Tag = 3.
             * Recibo del padre si todos los procesos convergen. Caso contrario, vuelvo a calcular. */
            MPI_Recv(&convergencia, 1, MPI_INT, source, 3, MPI_COMM_WORLD, &status);
        } while (!convergencia);

        /* Fin de la medicion de tiempo de los hijos */
        printf("Tiempo en segundos para convergencia %f para el hijo ID = %d\n", (dwalltime() - timetick), ID);
    }

    free(A);
    free(B);

    MPI_Finalize();
    return 0;
}