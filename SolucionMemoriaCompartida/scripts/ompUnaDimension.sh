#!/bin/bash  
# chmod +x script.sh

gcc -fopenmp -o una ../ompUnaDimensionReduccion.c 

for i in 1 2 4 

do

echo -e "----------------------"
echo "NUM_THREADS = $i"
echo -e "\n"

./una 512 $i
echo -e "\n"

./una 1024 $i
echo -e "\n"

./una 2048 $i
echo -e "\n"


done

echo -e "[Fin del Script] \n"