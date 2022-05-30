#!/bin/bash
#SBATCH -N 2
#SBATCH --exclusive
#SBATCH --tasks-per-node=4
#SBATCH -o directorioSalida/outputMpi2048_T8.txt
#SBATCH -e directorioSalida/erroresMpi2048_T8.txt

mpirun eje 2048
