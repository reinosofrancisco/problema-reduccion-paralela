# Resolucion de forma secuencial

Se modificaron todas las divisiones por sus correspondientes multiplicaciones para optimizar la velocidad de ejecucion.

Es decir Promedio[0,0] = ( A[0,0] + A[0,1] ) / 2

ahora es Promedio [0,0] = ( A[0,0] + A[0,1] ) * (0.5)

```
gcc -o output input.c
```