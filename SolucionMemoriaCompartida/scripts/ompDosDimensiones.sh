#!/bin/bash  
# chmod +x script.sh

gcc -fopenmp -o dos ../ompDosDimensionesReduccion.c 

for i in 1 2 4 

do

echo -e "----------------------"
echo "NUM_THREADS = $i"
echo -e "\n"

./dos 256 $i
echo -e "\n"

./dos 512 $i
echo -e "\n"

./dos 1024 $i
echo -e "\n"


done

echo -e "[Fin del Script] \n"