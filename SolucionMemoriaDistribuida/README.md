# Resolucion utilizando Memoria Distribuida - MPI

El archivo `mpiUnaDimensionReduccion.c` contiene la resolucion realizando:
* Pasaje del Vector A a todos los hilos.
* Pasaje de B[0] a todos los hilos.
* Cada hilo calcula su propia convergencia y le envia al padre la misma.
* El Padre verifica la convergencia de todos los hilos y avisa si deben seguir reduciendo el vector.

Modo de compilacion
```
    mpicc -o output input.c
```

Uso
```
    mpirun -np NUM_PROCESOS output
```
