# Resolucion utilizando Memoria Distribuida - MPI

El archivo `mpiUnaDimensionReduccion.c` y `mpiDosDimensionesReduccion.c` contienen la resolucion realizando:
* Pasaje del Vector/Matriz A por chunks a todos los hilos.
* Pasaje de B[0] a todos los hilos.
* Cada hilo calcula su propia convergencia y le envia al padre la misma.
* El Padre verifica la convergencia de todos los hilos y avisa si deben seguir reduciendo el vector.

Modo de compilacion
```
    mpicc -o output input.c
```

Uso
```
    mpirun -np NUM_PROCESOS output DIM
```

---

