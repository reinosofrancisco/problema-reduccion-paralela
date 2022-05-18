#include <stdio.h>
#include <stdbool.h>
#include <math.h>
#include <sys/time.h>
#include <stdint.h>
#include <stdlib.h>
#include <limits.h>

#define PRESICION 0.01

// Default Matrix Size
int DIM = 512;
float *A, *B;

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
            A[i * DIM + j] = (rand() % 2);
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

/** Guarda la Matriz (guardada por filas) en un Archivo.
 * @param A Matriz a guardar
 */
void print_txt_filas(float *A)
{
    FILE *fp = fopen("impresionMatriz.txt", "w");
    for (int i = 0; i < DIM; i++)
    {
        for (int j = 0; j < DIM; j++)
        {
            fprintf(fp, " | %f", A[i * DIM + j]);
        }
        fprintf(fp, "\n"); // Salto a la proxima Fila en el .txt
    }
    fclose(fp);
}

int main(int argc, char *argv[])
{

    // Variable auxiliar para calculo del tiempo
    double timetick;
    int i, j;

    /* Alocacion de memoria para Matriz N*N cuadrada */
    A = (float *)malloc(sizeof(float) * DIM * DIM);
    B = (float *)malloc(sizeof(float) * DIM * DIM);

    init_matrix_ceros_y_unos_filas(A);

    // Si pones esto, no converge nunca. Falta alguna posicion de memoria.
    // init_matrix_ceros_y_unos_filas(B);

    bool convergencia;

    // Uso Register ya que voy a acceder constantemente a aux
    register float aux;

    /* Inicio de la medicion de tiempo */
    timetick = dwalltime();

    print_txt_filas(A);

    do
    {

        /** Parte I - Calculo de Vector Reducido*/

        /** Hago una reduccion sin sentido para testear*/
        for (i = 0; i < DIM; i++)
        {
            for (j = 0; j < DIM; j++)
            {
                B[i * DIM + j] = (A[i * DIM + j] / 1.01);
            }
        }

        /**
         * @brief BUENO DE ESTA FORMA NO SE ROMPE NADA. DEBE HABER UN ERROR EN ASIGNAR MEMORIA EN LOS CASOS EXTREMOS.
         * 
         */


        /** Parte II - Verificacion de Convergencia. */

        convergencia = true;

        for (i = 0; i < DIM; i++)
        {
            for (j = 0; j < DIM; j++)
            {
                if (fabs(B[0] - B[i * DIM + j]) > PRESICION)
                {
                    convergencia = false;
                }
            }
        }

        if (!convergencia)
        {
            printf("El resultado no converge | Recalculando...\n");

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