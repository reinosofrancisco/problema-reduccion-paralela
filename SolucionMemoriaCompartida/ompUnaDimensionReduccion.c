#include <stdio.h>
#include <math.h>
#include <sys/time.h>
#include <stdint.h>
#include <stdlib.h>
#include <limits.h>
#include <omp.h>

#define PRESICION 0.01

// Default Vector Size
int DIM = 2048;
float *A, *B;

/***************************************************************************/
/*                       Funciones Auxiliares                              */
/***************************************************************************/

/** Inicializa el Vector A con valores Aleatorios del 0 a 1
 * @param A Vector a inicializar
 */
void init_vector_ceros_y_unos(float *A)
{
    for (int i = 0; i < DIM; i++)
    {
        A[i] = rand() / (float)RAND_MAX;
    }
}

/** Funcion para comparar tiempos */
double dwalltime()
{
    double sec;
    struct timeval tv;

    gettimeofday(&tv, NULL);
    sec = tv.tv_sec + tv.tv_usec / 1000000.0;
    return sec;
}

/***************************************************************************/
/*                       Codigo Principal                                  */
/***************************************************************************/

int main(int argc, char *argv[])
{
    // Variable auxiliar para calculo del tiempo
    double timetick;
    int numThreads;

    // Controla los argumentos al programa
    if ((argc != 3) || ((DIM = atoi(argv[1])) <= 0) || ((numThreads = atoi(argv[2])) <= 0))
    {
        printf("\nUsar: %s DIM NUM_THREADS\n  DIM: Dimension del Vector\n  NUM_THREADS: Numero de threads\n", argv[0]);
        exit(1);
    }

    // Aloca memoria para el vector principal y el Vector Resultado
    A = (float *)malloc(sizeof(float) * DIM);
    B = (float *)malloc(sizeof(float) * DIM);

    init_vector_ceros_y_unos(A);

    printf("Vector Original de size %d\n", DIM);

    int convergencia, i;
    omp_set_num_threads(numThreads);
    float *temp;


    /* Inicio de la medicion de tiempo */
    timetick = dwalltime();


    #pragma omp parallel
    {
        do
        {
            /** Parte I - Calculo de Vector Reducido */

            #pragma omp for private(i)
            for (i = 0; i < DIM; i++)
            {
                if (i == 0)
                {
                    B[i] = (A[i] + A[i + 1]) * 0.5;
                }
                else if (i == DIM - 1)
                {
                    B[i] = (A[i - 1] + A[i]) * 0.5;
                }
                else
                {
                    B[i] = (A[i - 1] + A[i] + A[i + 1]) * 0.333333;
                }
            } // No hay Join implicito.
            

            

            /** Parte II - Verificacion de Convergencia. */
            #pragma omp single 
            {
                convergencia = 1;
            }

            // Se deben esperar despues del for porque todos necesitan el valor de B[0] y de convergencia
            #pragma omp barrier

            #pragma omp for private(i) reduction(&& : convergencia)
            for (i = 0; i < DIM; i++)
            {
                convergencia = convergencia && (fabs(B[0] - B[i]) < PRESICION);
            } // Threads Join because of reduction statement.


            #pragma omp master
            {
                if (!convergencia)
                {
                    // Hago un swap de los vectores y vuelvo a usar B como auxiliar. 
                    temp = A;
                    A = B;
                    B = temp; 
                }
            }

            // Se deben esperar despues del for porque todos necesitan las puntas de A
            #pragma omp barrier 

        } while (!convergencia);

    } // Fin de la region paralela. Join implicito.

    /* Fin de la medicion de tiempo */
    printf("Tiempo en segundos para convergencia %f\n", dwalltime() - timetick);

    free(A);
    free(B);
    return 0;
}
