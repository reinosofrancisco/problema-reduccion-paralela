#!/bin/bash  
# chmod +x script.sh

mpicc -o una ../mpiUnaDimensionReduccion.c 

for i in 2 4 

do

echo -e "----------------------"
echo "NUM_THREADS = $i"
echo -e "\n"

mpirun -np $i una 256
echo -e "\n"

mpirun -np $i una 512
echo -e "\n"

mpirun -np $i una 1024
echo -e "\n"

# mpirun -np $i una 2048
# echo -e "\n"

done

echo -e "[Fin del Script] \n"