# Problema de Reducción 

Se aplica la función promedio hasta alcanzar un valor aproximado de convergencia. <br>
Se resolvera en una y dos dimensiones utilizando el modelo de Memoria Compartida (OpenMP) y el modelo de Memoria Distribuida (MPI).

---

## Caso 1: Una dimension

Se parte de un arreglo V de N elementos de tipo float. <br>
Para cada elemento en la posición i del vector V debe calcularse el 
promedio entre el valor actual y sus vecinos próximos. <br>

<img src="Problematica%20y%20Datos%20Auxiliares/images/4171f11099a0318d0658b6994d2b12d4af5636007695c21a037fcb43a1d22726.png" alt="drawing" width="450"/>

<!-- ![picture 1](images/4171f11099a0318d0658b6994d2b12d4af5636007695c21a037fcb43a1d22726.png)   -->

Los casos especiales del primer y último elemento deben calcularse de la siguiente manera:
<!-- 
![picture 2](images/3cc63fd8073f185706017fbdcc079d3ca4463f75e569821509d5994d9dfdcfe8.png)   -->
<img src="Problematica%20y%20Datos%20Auxiliares/images/3cc63fd8073f185706017fbdcc079d3ca4463f75e569821509d5994d9dfdcfe8.png" alt="drawing" width="450"/>

---

## Caso 2: Dos dimensiones

Se parte de una matriz V de NxN elementos de tipo float.
Para cada elemento en la posición (i,j) de la matriz V debe calcularse el promedio entre el valor actual y sus vecinos próximos, esto es:

<!-- ![picture 5](images/65b145f83bb1739b3d6805e5c3d3ecbec6dc8e8be2d07726366c2f9a46bf6abb.png)   -->
<img src="Problematica%20y%20Datos%20Auxiliares/images/65b145f83bb1739b3d6805e5c3d3ecbec6dc8e8be2d07726366c2f9a46bf6abb.png" alt="drawing" width="450"/>

Los casos especiales son las esquinas y los laterales de la matriz. En estos casos, se debe seguir la misma estrategia que en una dimensión: considerar sólo los vecinos próximos existentes y dividir por la cantidad de valores utilizados (esquinas dividir por 4 y laterales dividir por 6).

---

## Pautas de Convergencia

Una vez realizada una reducción, se debe verificar que todos los valores del vector convergen a un valor similar. <br>
Para verificar la convergencia se toma el primer elemento y se
compara con el resto de los elementos del vector. Si la diferencia en valor absoluto del valor del primer elemento con todos los elementos restantes es menor a un valor de precisión el algoritmo converge, en caso contrario el algoritmo no converge:

<!-- ![picture 4](images/1763e84ea453dbb23ca5aa4808c60e0f940dffc341345784546a6cbdd81c5afc.png)   -->
<img src="Problematica%20y%20Datos%20Auxiliares/images/1763e84ea453dbb23ca5aa4808c60e0f940dffc341345784546a6cbdd81c5afc.png" alt="drawing" width="450"/>

Para el caso de dos dimensiones se toma el primer elemento V[0,0].

Si los valores convergen el algoritmo finaliza. **Si los valores no convergen debe aplicarse la reducción nuevamente.** Esto puede implicar varias ejecuciones de la reducción hasta que los
valores convergen.

---

# Resultados obtenidos en el Cluster

## Tiempos de Ejecucion

![picture 1](Problematica%20y%20Datos%20Auxiliares/images/86a182938a4755264fb91ffb125e997998973cf7b1e98c8abda3c6aaa0220870.png)  


## Speedup

 ![picture 2](Problematica%20y%20Datos%20Auxiliares/images/3d7b7fbacc9b23fd7881a6c6f41b280a55bebe463a9bdab949e849b3f19c6eca.png)  


## Eficiencia

![picture 3](Problematica%20y%20Datos%20Auxiliares/images/9d9aa47aa7d6db8448b9ab76a11db439380b4677f3fbf55ef9d683087787d850.png)  

