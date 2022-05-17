#include <stdio.h>
#include <sys/time.h>
#include <stdbool.h>

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

    /* Alocacion de memoria para Matriz N*N cuadrada */
    A = (double *)malloc(sizeof(double) * DIM * DIM);
    B = (double *)malloc(sizeof(double) * DIM * DIM);

    init_matrix_ceros_y_unos_filas(A);

    bool convergencia;

    /* Inicio de la medicion de tiempo */
    timetick = dwalltime();

    // Uso Register ya que voy a acceder constantemente a aux
    register double aux;

    timetick = dwalltime();

    do
    {

        /** Parte I - Calculo de Vector Reducido */
        for (int i = 0; i < DIM; i++)
        {
            for (int j = 0; j < DIM; j++)
            {
                for (int k = 0; k < DIM; k++)
                {
                    /*
                    A[i,j],     A[i,j-1],       A[i,j+1],
                    A[i-1,j],   A[i-1,j-1],     A[i-1,j+1],
                    A[i+1,j],   A[i+1,j-1],     A[i+1,j+1]
                    Calculo del promedio con los 4-6-8 vecinos + el valor A[pos,pos] */

                    /* Exceptuar los casos de i = 0, i = DIM, j = 0 y j = DIM ya que son extremos*/
                }
            }
        }

        /** Parte II - Verificacion de Convergencia. */

        convergencia = true;

        for (int i = 0; i < DIM; i++)
        {
            for (int j = 0; j < DIM; j++)
            {
                for (int k = 0; k < DIM; k++)
                {
                    /* if (fabs(B[0] - B[i,j]) > PRESICION) { convergencia = false } */
                }
            }
        }

        if (!convergencia)
        {
            printf("El resultado no converge | Recalculando...\n");

            // Copio toda la matriz B en A, y vuelvo a utilizar B como auxiliar
            for (int i = 0; i < DIM; i++)
            {
                for (int j = 0; j < DIM; j++)
                {
                    for (int k = 0; k < DIM; k++)
                    {
                        /* A[i,j] = B[i,j] */
                    }
                }
            }
        }
    } while (!convergencia);

    /* Fin de la medicion de tiempo */
    printf("Tiempo en segundos para convergencia %f\n", dwalltime() - timetick);

    free(A);
    free(B);
}