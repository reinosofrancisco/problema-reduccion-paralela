#!/bin/bash  
# chmod +x script.sh

gcc -o dos ../reduccionDosDimensiones.c 


echo -e "----------------------"
echo "NUM_THREADS = 1 (Sequencial)"
echo -e "\n"

./dos 256
echo -e "\n"

./dos 512
echo -e "\n"

./dos 1024
echo -e "\n"

echo -e "[Fin del Script] \n"