#!/bin/bash
#SBATCH -N 1
#SBATCH --exclusive
#SBATCH -o directorioSalida/outputOmp1024_4.txt
#SBATCH -e directorioSalida/erroresOmp1024_4.txt
export OMP_NUM_THREADS=4
./eje 1024 4

