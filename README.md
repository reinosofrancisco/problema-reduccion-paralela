<div align="center">
  <h2><strong>Problema de Reducción Paralela</strong></h2>

  <a href="https://github.com/reinosofrancisco/problema-reduccion-paralela"><img src="https://img.shields.io/badge/MPI%20%26%20OPEN--MP-V1.0-blue?style=plastic&logo=data:image/png;base64,iVBORw0KGgoAAAANSUhEUgAAABAAAAAQCAMAAAAoLQ9TAAAABGdBTUEAALGPC/xhBQAAACBjSFJNAAB6JQAAgIMAAPn/AACA6QAAdTAAAOpgAAA6mAAAF2+SX8VGAAACdlBMVEX59PSeODuvW16wXF+eNzurUVSqUFOeNzqwXV+wXF6dNjmdNTidNTmwW1728fG1ZGayXmC+eXzKk5XKkpTIj5HFh4m9eHqzY2W0ZmivWl28dXjRq6zKk5W8dnixX2HCgYO+e32vXF6xYGK+d3m6cnS4bnCxXmDTrq/Ml5iyYmS/eny9d3nFj5HIjpDTpqfUp6jJk5XLlpe8d3m9eHqyX2HLpqfMp6i4Zmm6dHa7dHe2ZWexXF++fX++foDLpaaxXF63ZWi8eHrDjY/Im53Im5y7dniyXmHJvr7RysrTpafIjpDRoqPOm53TwMDDv7+3j5CjU1afOz6dNTmpT1K6cXPIjo/OmpzOu7u5rq6hUlWOFRmKDRGVJSi6cnTDhYerU1WlRknEhojLlJXGwsKjVVeJDBDSo6T06ur06em3a26oTE/Qn6HSxsewiYqJDRGRHCDKk5T38PDw4uLy5ebIjY+MERWNFBi9eHrWq63NysqnWVyJCw+JCxC8dnjs2drx4+P37+/as7SYLC+nSk3VqKr07u+kRUiICw+1Z2nx5OTq1NXQoKGcNDehPUH69PWICg6MEhbLlZbu3t7t29zbtbbct7ijQ0aHCQ2iP0P48fLt6uq0Zmjr1tfgwcLkycrx5OXmzM2gOz6HBwuxX2H18fLXzM3Op6jlysvdurvUp6iNExbKn6Haz9Db2NiyZGabMTXCgoTIj5GmR0qOFhqLDxOwXmDg29zSv8Da0NCyY2WVIyeVJSmNExeHCAyICQ2MExexYGLh1dbZx8fTwMHc2NnOpqixYWSjQUSxYGPPpabayMnd09Pc2drg2drc1tfc2dng1db///9DmxmyAAAAR3RSTlMAAAAAAAAAAAAAAAAAAAAER6bj+vrjpkcEFpHu7pEWr7AWBJGRBUfu7kimp+Pj+/vj46enSO7vBZKSBRewse8XBajk+/uoSR9YVxAAAAABYktHRNF50f8KAAAACXBIWXMAAC4jAAAuIwF4pT92AAAAB3RJTUUH5ggMEC0mmG7aJAAAARtJREFUGNMBEAHv/gAAAQIPEBESExQVFhcYAwQAAAEFGRobR0hJSktMHB0eBgcAAhkfTU5PUFFSU1RVViAhCAAiI1dYWVpbXF1eX2BhYiQlACYnY2RbZV1mZ2hpZWprKCkAKmxtWm5vcHFyc3R1dnd4KwAseXp7fH1+f4CBgmV7g4QtAC6FhoduiIl+iouMW3uNZy8AL45gj5CRkpOUlYGWl5iZLwAwmpuPZX2cnZ6foKGio6QxADKlplqPfaeonamDkKqrrDMANDWtro+vsLGys7SPtbY2NAA3OLe4ubq7vL2+v8DBwjk6AAk7PMPExcbHx8jJtso9OwkACgU7OD7LzM3Oz9A2OD8FCwAADA1ANEFCQ0RCRUY6DQsO2AdkEf2XVzoAAAAldEVYdGRhdGU6Y3JlYXRlADIwMjItMDgtMTJUMTY6NDU6MzYrMDA6MDCtLuhjAAAAJXRFWHRkYXRlOm1vZGlmeQAyMDIyLTA4LTEyVDE2OjQ1OjM2KzAwOjAw3HNQ3wAAAABJRU5ErkJggg=="/></a>   




Se aplica la función promedio hasta alcanzar un valor aproximado de convergencia. <br>
Se resolvera en una y dos dimensiones utilizando el modelo de Memoria Compartida (OpenMP) y el modelo de Memoria Distribuida (MPI).

---

<br>

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


</div>
