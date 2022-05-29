#!/bin/bash  
# chmod +x script.sh

gcc -o una ../reduccionUnaDimension.c 


echo -e "----------------------"
echo "NUM_THREADS = 1 (Sequencial)"
echo -e "\n"

./una 512
echo -e "\n"

./una 1024
echo -e "\n"

./una 2048
echo -e "\n"

echo -e "[Fin del Script] \n"