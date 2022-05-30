#!/bin/bash
#SBATCH -N 1
#SBATCH --exclusive
#SBATCH -o directorioSalida/outputOmp2048_8.txt
#SBATCH -e directorioSalida/erroresOmp2048_8.txt
export OMP_NUM_THREADS=8
./eje 2048

