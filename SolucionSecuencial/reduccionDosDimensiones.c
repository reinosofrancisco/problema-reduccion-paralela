#include <stdio.h>
#include <stdbool.h>
#include <math.h>
#include <sys/time.h>
#include <stdint.h>
#include <stdlib.h>
#include <limits.h>

#define PRESICION 0.01

// Default Matrix Size
int DIM = 32;
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

        /* Primero calculo para los casos comunes,
            desde i = 1 hasta i = DIM - 1,
            y desde j = 1 hasta j = DIM - 1.
            */
        for (i = 1; i < DIM - 1; i++)
        {
            for (j = 1; j < DIM - 1; j++)
            {
                aux = 0;
                aux += (A[i * DIM + j] + A[i * DIM + (j - 1)] + A[i * DIM + (j + 1)]);
                aux += (A[(i - 1) * DIM + j] + A[(i - 1) * DIM + (j - 1)] + A[(i - 1) * DIM + (j + 1)]);
                aux += (A[(i + 1) * DIM + j] + A[(i + 1) * DIM + (j - 1)] + A[(i + 1) * DIM + (j + 1)]);
                B[i * DIM + j] = (aux / 9);

                // B[i * DIM + j] = (A[i * DIM + j] + A[i * DIM + (j - 1)] + A[i * DIM + (j + 1)]
                //                 + A[(i - 1) * DIM + j] + A[(i - 1) * DIM + (j - 1)] + A[(i - 1) * DIM + (j + 1)]
                //                 + A[(i + 1) * DIM + j] + A[(i + 1) * DIM + (j - 1)] + A[(i + 1) * DIM + (j + 1)]) / 9;
                /*
                A[i-1,j],   A[i-1,j-1],     A[i-1,j+1],
                A[i,j],     A[i,j-1],       A[i,j+1],
                A[i+1,j],   A[i+1,j-1],     A[i+1,j+1]
                Calculo del promedio con los 4-6-8 vecinos + el valor A[pos,pos] */

                /* Exceptuar los casos de i = 0, i = DIM, j = 0 y j = DIM ya que son extremos*/
            }
        }

        /** CALCULO PARA LA PRIMER Y ULTIMA FILA, Y LA PRIMER Y ULTIMA COLUMNA. SIN TOMAR LAS PUNTAS*/

        /* Calculo para i = 0 sin tomar las puntas. Esto toma la primer fila */
        i = 0;
        for (j = 1; j < DIM - 1; j++)
        {
            aux = 0;
            aux += (A[i * DIM + j] + A[i * DIM + (j - 1)] + A[i * DIM + (j + 1)]);
            aux += (A[(i + 1) * DIM + j] + A[(i + 1) * DIM + (j - 1)] + A[(i + 1) * DIM + (j + 1)]);
            B[i * DIM + j] = (aux / 6);
        }

        /* Calculo para i = DIM sin tomar las puntas. Esto toma la ultima fila */
        i = DIM;
        for (j = 1; j < DIM - 1; j++)
        {
            aux = 0;
            aux += (A[i * DIM + j] + A[i * DIM + (j - 1)] + A[i * DIM + (j + 1)]);
            aux += (A[(i - 1) * DIM + j] + A[(i - 1) * DIM + (j - 1)] + A[(i - 1) * DIM + (j + 1)]);
            B[i * DIM + j] = (aux / 6);
        }

        /* Calculo para j = 0 sin tomar las puntas. Esto toma la primer columna */
        j = 0;
        for (i = 1; i < DIM - 1; i++)
        {
            aux = 0;
            aux += (A[i * DIM + j] + A[(i - 1) * DIM + j] + A[(i + 1) * DIM + j]);
            aux += (A[i * DIM + (j + 1)] + A[(i - 1) * DIM + (j + 1)] + A[(i + 1) * DIM + (j + 1)]);
            B[i * DIM + j] = (aux / 6);
        }

        /* Calculo para j = DIM sin tomar las puntas. Esto toma la ultima columna */
        j = DIM;
        for (i = 1; i < DIM - 1; i++)
        {
            aux = 0;
            aux += (A[i * DIM + j] + A[(i - 1) * DIM + j] + A[(i + 1) * DIM + j]);
            aux += (A[i * DIM + (j - 1)] + A[(i - 1) * DIM + (j - 1)] + A[(i + 1) * DIM + (j - 1)]);
            B[i * DIM + j] = (aux / 6);
        }

        /** CALCULO PARA LAS PUNTAS */

        /* Calculo para i = 0 y j = 0. Esto toma la primer casilla [0,0] */
        i = 0;
        j = 0;
        B[i * DIM + j] = (A[i * DIM + j] + A[(i + 1) * DIM + j] + A[i * DIM + (j + 1)] + A[(i + 1) * DIM + (j + 1)]) / 4;

        /* Calculo para i = 0 y j = DIM. Esto toma la ultima casilla [0,DIM]*/
        i = 0;
        j = DIM;
        B[i * DIM + j] = (A[i * DIM + j] + A[(i + 1) * DIM + j] + A[i * DIM + (j - 1)] + A[(i + 1) * DIM + (j - 1)]) / 4;

        /* Calculo para i = DIM y j = 0. Esto toma la primer casilla [DIM,0]*/
        i = DIM;
        j = 0;
        B[i * DIM + j] = (A[i * DIM + j] + A[(i - 1) * DIM + j] + A[i * DIM + (j + 1)] + A[(i - 1) * DIM + (j + 1)]) / 4;

        /* Calculo para i = DIM y j = DIM. Esto toma la ultima casilla [DIM,DIM]*/
        i = DIM;
        j = DIM;
        B[i * DIM + j] = (A[i * DIM + j] + A[(i - 1) * DIM + j] + A[i * DIM + (j - 1)] + A[(i - 1) * DIM + (j - 1)]) / 4;

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