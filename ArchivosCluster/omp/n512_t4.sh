#!/bin/bash
#SBATCH -N 1
#SBATCH --exclusive
#SBATCH -o directorioSalida/outputOmp512_4.txt
#SBATCH -e directorioSalida/erroresOmp512_4.txt
export OMP_NUM_THREADS=4
./eje 512
