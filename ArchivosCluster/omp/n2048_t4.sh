#!/bin/bash
#SBATCH -N 1
#SBATCH --exclusive
#SBATCH -o directorioSalida/outputOmp2048_4.txt
#SBATCH -e directorioSalida/erroresOmp2048_4.txt
export OMP_NUM_THREADS=4
./eje 2048

