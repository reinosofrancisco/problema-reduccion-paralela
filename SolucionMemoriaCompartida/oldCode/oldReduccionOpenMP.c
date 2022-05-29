#include <stdio.h>
#include <math.h>
#include <sys/time.h>
#include <stdint.h>
#include <stdlib.h>
#include <limits.h>
#include <omp.h>

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
            A[i * DIM + j] = (rand() / (float)(RAND_MAX));
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

    // Variable auxiliares
    int numThreads;
    double timetick;
    int i, j;
    int convergencia;
    register float aux; // Register for multiple accesses to the same variable

    // Controla los argumentos al programa
    if ((argc != 3) || ((DIM = atoi(argv[1])) <= 0) || ((numThreads = atoi(argv[2])) <= 0))
    {
        printf("\nUsar: %s DIM numThreads\n  DIM: Dimension de la Matriz (DIM * DIM)\n  numThreads: Numero de threads\n", argv[0]);
        exit(1);
    }
    

    /* Alocacion de memoria para Matriz N*N cuadrada */
    A = (float *)malloc(sizeof(float) * DIM * DIM);
    B = (float *)malloc(sizeof(float) * DIM * DIM);

    init_matrix_ceros_y_unos_filas(A);

    /* Seteo el Numero de Threads a utilizar. */
    omp_set_num_threads(numThreads);

    /* Inicio de la medicion de tiempo */
    timetick = dwalltime();

    do
    {

        /** Parte I - Calculo de Vector Reducido*/

        /* Primero calculo para los casos comunes, i = 1 a i = DIM - 2 | j = 1 a j = DIM - 2 */
        #pragma omp parallel for private(i, j,aux) 
        for (i = 1; i < DIM - 1; i++)
        {
            for (j = 1; j < DIM - 1; j++)
            {
                aux = 0;
                aux += (A[i * DIM + j] + A[i * DIM + (j - 1)] + A[i * DIM + (j + 1)]);
                aux += (A[(i - 1) * DIM + j] + A[(i - 1) * DIM + (j - 1)] + A[(i - 1) * DIM + (j + 1)]);
                aux += (A[(i + 1) * DIM + j] + A[(i + 1) * DIM + (j - 1)] + A[(i + 1) * DIM + (j + 1)]);
                B[i * DIM + j] = (aux * 0.111111);
            }
        }//Barrera implicita de OpenMP

        /** CALCULO PARA LA PRIMER Y ULTIMA FILA,
         * Y LA PRIMER Y ULTIMA COLUMNA.
         * SIN TOMAR LAS PUNTAS
         */

        

        /* Calculo para i = 0 sin tomar las puntas. Esto toma la primer fila */
        i = 0;
        for (j = 1; j < DIM - 1; j++)
        {
            aux = 0;
            aux += (A[i * DIM + j] + A[i * DIM + (j - 1)] + A[i * DIM + (j + 1)]);
            aux += (A[(i + 1) * DIM + j] + A[(i + 1) * DIM + (j - 1)] + A[(i + 1) * DIM + (j + 1)]);
            B[i * DIM + j] = (aux * 0.166666);
        }

        /* Calculo para i = DIM - 1 sin tomar las puntas. Esto toma la ultima fila */
        i = DIM - 1;
        for (j = 1; j < DIM - 1; j++)
        {
            aux = 0;
            aux += (A[i * DIM + j] + A[i * DIM + (j - 1)] + A[i * DIM + (j + 1)]);
            aux += (A[(i - 1) * DIM + j] + A[(i - 1) * DIM + (j - 1)] + A[(i - 1) * DIM + (j + 1)]);
            B[i * DIM + j] = (aux * 0.166666);
        }

        /* Calculo para j = 0 sin tomar las puntas. Esto toma la primer columna */
        j = 0;
        for (i = 1; i < DIM - 1; i++)
        {
            aux = 0;
            aux += (A[i * DIM + j] + A[(i - 1) * DIM + j] + A[(i + 1) * DIM + j]);
            aux += (A[i * DIM + (j + 1)] + A[(i - 1) * DIM + (j + 1)] + A[(i + 1) * DIM + (j + 1)]);
            B[i * DIM + j] = (aux * 0.166666);
        }

        /* Calculo para j = DIM - 1 sin tomar las puntas. Esto toma la ultima columna */
        j = DIM - 1;
        for (i = 1; i < DIM - 1; i++)
        {
            aux = 0;
            aux += (A[i * DIM + j] + A[(i - 1) * DIM + j] + A[(i + 1) * DIM + j]);
            aux += (A[i * DIM + (j - 1)] + A[(i - 1) * DIM + (j - 1)] + A[(i + 1) * DIM + (j - 1)]);
            B[i * DIM + j] = (aux * 0.166666);
        }

        /** CALCULO PARA LAS PUNTAS */

        /* Calculo para i = 0 y j = 0. Esto toma la primer casilla [0,0] */
        i = 0;
        j = 0;
        B[i * DIM + j] = (A[i * DIM + j] + A[(i + 1) * DIM + j] + A[i * DIM + (j + 1)] + A[(i + 1) * DIM + (j + 1)]) * (0.25);

        /* Calculo para i = 0 y j = DIM - 1. Esto toma la ultima casilla [0,DIM - 1]*/
        i = 0;
        j = DIM - 1;
        B[i * DIM + j] = (A[i * DIM + j] + A[(i + 1) * DIM + j] + A[i * DIM + (j - 1)] + A[(i + 1) * DIM + (j - 1)]) * (0.25);

        /* Calculo para i = DIM - 1 y j = 0. Esto toma la primer casilla [DIM - 1,0]*/
        i = DIM - 1;
        j = 0;
        B[i * DIM + j] = (A[i * DIM + j] + A[(i - 1) * DIM + j] + A[i * DIM + (j + 1)] + A[(i - 1) * DIM + (j + 1)]) * (0.25);

        /* Calculo para i = DIM - 1 y j = DIM - 1. Esto toma la ultima casilla [DIM - 1,DIM - 1]*/
        i = DIM - 1;
        j = DIM - 1;
        B[i * DIM + j] = (A[i * DIM + j] + A[(i - 1) * DIM + j] + A[i * DIM + (j - 1)] + A[(i - 1) * DIM + (j - 1)]) * (0.25);

        /** Parte II - Verificacion de Convergencia. */

        convergencia = 1;

        /* No puedo paralelizar la verificacion a menos que tenga un vector[DIM]
        de booleanos, y guarde dada true/false, para luego recorrerlo y verificar
        si alguno de los threads devolvio false.*/
        #pragma omp parallel for reduction(&&:convergencia)
        for (i = 0; i < DIM; i++)
        {
            for (j = 0; j < DIM; j++)
            {
                convergencia = convergencia && (fabs(B[0] - B[i]) < PRESICION);
            }
        }

        if (!convergencia)
        {
            // Copio toda la matriz B en A, y vuelvo a utilizar B como auxiliar
            #pragma omp parallel for private(i,j)
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