#!/bin/bash
#SBATCH -N 1
#SBATCH --exclusive
#SBATCH -o directorioSalida/outputOmp1024_8.txt
#SBATCH -e directorioSalida/erroresOmp1024_8.txt
export OMP_NUM_THREADS=8
./eje 1024 8

