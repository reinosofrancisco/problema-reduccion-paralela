#!/bin/bash
#SBATCH -N 2
#SBATCH --exclusive
#SBATCH --tasks-per-node=2
#SBATCH -o directorioSalida/outputMpi2048_T4.txt
#SBATCH -e directorioSalida/erroresMpi2048_T4.txt

mpirun eje 2048
