#include <stdio.h>
#include <math.h>
#include <sys/time.h>
#include <stdint.h>
#include <stdlib.h>
#include <limits.h>

#define PRESICION 0.01

#define true 1
#define false 0
#define bool int

// Default Matrix Size
int DIM = 8;
float *A, *B;

/***************************************************************************/
/*                       Funciones Auxiliares                              */
/***************************************************************************/

/** Inicializa la Matriz con valores Aleatorios del 1 al 100
 * guardada por filas. Acceso mediante A[i * DIM + j]
 * @param A Matriz a inicializar
 */
void init_matrix_ceros_y_unos_filas(float *A)
{
    for (int i = 0; i < DIM; i++)
    {
        for (int j = 0; j < DIM; j++)
        {
            A[i * DIM + j] = rand() / (float)RAND_MAX;
        }
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

    // Controla los argumentos al programa
    if ((argc != 2) || ((DIM = atoi(argv[1])) <= 0))
    {
        printf("\nUsar: %s DIM\n  DIM: Dimension de la Matriz (DIM * DIM)\n", argv[0]);
        exit(1);
    }

    // Variable auxiliares
    double timetick;
    int i, j;
    bool convergencia;
    register float aux; // Register for multiple accesses to the same variable

    /* Alocacion de memoria para Matriz N*N cuadrada */
    A = (float *)malloc(sizeof(float) * DIM * DIM);
    B = (float *)malloc(sizeof(float) * DIM * DIM);

    init_matrix_ceros_y_unos_filas(A);

    /* Inicio de la medicion de tiempo */
    timetick = dwalltime();

    do
    {

        /** Parte I - Calculo de Vector Reducido*/

        for (i = 0; i < DIM; i++)
        {
            for (j = 0; j < DIM; j++)
            {
                if (i == 0 && j == 0)
                {
                    // Primer cubito Primer fila
                    B[i * DIM + j] = (A[i * DIM + j] + A[(i + 1) * DIM + j] + A[i * DIM + (j + 1)] + A[(i + 1) * DIM + (j + 1)]) * (0.25);
                }
                else if (i == 0 && j == DIM - 1)
                {
                    // Ultimo cubito Primer fila
                    B[i * DIM + j] = (A[i * DIM + j] + A[(i + 1) * DIM + j] + A[i * DIM + (j - 1)] + A[(i + 1) * DIM + (j - 1)]) * (0.25);
                }
                else if (i == DIM - 1 && j == 0)
                {
                    // Primer cubito Ultima fila
                    B[i * DIM + j] = (A[i * DIM + j] + A[(i - 1) * DIM + j] + A[i * DIM + (j + 1)] + A[(i - 1) * DIM + (j + 1)]) * (0.25);
                }
                else if (i == DIM - 1 && j == DIM - 1)
                {
                    // Ultimo cubito Ultima fila
                    B[i * DIM + j] = (A[i * DIM + j] + A[(i - 1) * DIM + j] + A[i * DIM + (j - 1)] + A[(i - 1) * DIM + (j - 1)]) * (0.25);
                }
                else if (i == 0)
                {
                    // Primer fila sin puntas
                    aux = 0;
                    aux += (A[i * DIM + j] + A[i * DIM + (j - 1)] + A[i * DIM + (j + 1)]);
                    aux += (A[(i + 1) * DIM + j] + A[(i + 1) * DIM + (j - 1)] + A[(i + 1) * DIM + (j + 1)]);
                    B[i * DIM + j] = (aux * 0.166666);
                }
                else if (i == DIM - 1)
                {
                    // Ultima fila sin puntas
                    aux = 0;
                    aux += (A[i * DIM + j] + A[i * DIM + (j - 1)] + A[i * DIM + (j + 1)]);
                    aux += (A[(i - 1) * DIM + j] + A[(i - 1) * DIM + (j - 1)] + A[(i - 1) * DIM + (j + 1)]);
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
                };
            }
        }

        /** Parte II - Verificacion de Convergencia. */

        convergencia = true;

        for (i = 0; i < DIM; i++)
        {
            for (j = 0; j < DIM; j++)
            {
                convergencia = convergencia && (fabs(B[0] - B[i * DIM + j]) < PRESICION);
            }
        }

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
    } while (!convergencia);

    /* Fin de la medicion de tiempo */
    printf("Tiempo en segundos para convergencia %f\n", dwalltime() - timetick);

    free(A);
    free(B);
}