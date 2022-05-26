#include <stdio.h>
#include <stdbool.h>
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
    int numThreads = 4;

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

    bool convergencia;
    omp_set_num_threads(numThreads);

    /* Inicio de la medicion de tiempo */
    timetick = dwalltime();

    do
    {
        int i;

        /** Parte I - Calculo de Vector Reducido */
        
        #pragma omp parallel for private(i)
        for (int i = 0; i < DIM; i++)
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
        }

        /** Parte II - Verificacion de Convergencia. */

        convergencia = true;

        #pragma omp parallel for private(i) reduction(&& : convergencia)
        for (i = 0; i < DIM; i++)
        {
            convergencia = convergencia && (fabs(B[0] - B[i]) < PRESICION);
        }

        if (!convergencia)
        {
            //  Copio todo el Vector B en A, y vuelvo a utilizar B como auxiliar
            for (int i = 0; i < DIM; i++)
            {
                A[i] = B[i];
            }
        }

    } while (!convergencia);

    /* Fin de la medicion de tiempo */
    printf("Tiempo en segundos para convergencia %f\n", dwalltime() - timetick);

    free(A);
    free(B);
    return 0;
}