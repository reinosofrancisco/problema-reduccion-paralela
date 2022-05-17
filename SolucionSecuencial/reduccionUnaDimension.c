#include <stdio.h>
#include <stdbool.h>
#include <math.h>
#include <sys/time.h>
#include <stdint.h>
#include <stdlib.h>
#include <limits.h>

#define PRESICION 0.01

// Default Vector Size
int DIM = 512;
float *A, *B;

/** Inicializa el Vector A con valores Aleatorios del 1 al 100
 * @param A Vector a inicializar
 */
void init_vector_ceros_y_unos(float *A)
{
    for (int i = 0; i < DIM; i++)
    {
        A[i] = (rand() % 2);
    }
}

/** Imprime el Vector enviado 
 * @param A Vector a imprimir
 */
void print_vector(float *vector)
{
    for (int i = 0; i < DIM; i++)
    {
        printf("%f ", vector[i]);
    }
    printf("\n");
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

int main(int argc, char *argv[])
{

    // Variable auxiliar para calculo del tiempo
    double timetick;

    // Aloca memoria para el vector principal y el Vector Resultado
    A = (float *)malloc(sizeof(float) * DIM);
    B = (float *)malloc(sizeof(float) * DIM);

    init_vector_ceros_y_unos(A);

    printf("Vector A: \n");
    print_vector(A);
    printf("\n");

    bool convergencia;

    /* Inicio de la medicion de tiempo */
    timetick = dwalltime();

    do
    {
        /** Parte I - Calculo de Vector Reducido */

        for (int i = 1; i < DIM - 1; i++)
        {
            B[i] = (A[i - 1] + A[i] + A[i + 1]) / 3;
        }

        // Casos extremos
        B[0] = (A[0] + A[1]) / 2;
        B[DIM] = (A[DIM - 1] + A[DIM]) / 2;

        /** Parte II - Verificacion de Convergencia. */

        convergencia = true;

        for (int i = 0; i < DIM; i++)
        {
            if (fabs(B[0] - B[i]) > PRESICION)
            {
                convergencia = false;
            }
        }

        if (!convergencia)
        {
            printf("El resultado no converge | Recalculando...\n");

            // Copio todo el Vector B en A, y vuelvo a utilizar B como auxiliar
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
}