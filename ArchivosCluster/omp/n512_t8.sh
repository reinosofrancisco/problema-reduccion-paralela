#!/bin/bash
#SBATCH -N 1
#SBATCH --exclusive
#SBATCH -o directorioSalida/outputOmp512_8.txt
#SBATCH -e directorioSalida/erroresOmp512_8.txt
export OMP_NUM_THREADS=8
./eje 512
