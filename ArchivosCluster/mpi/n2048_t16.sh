#!/bin/bash
#SBATCH -N 2
#SBATCH --exclusive
#SBATCH --tasks-per-node=8
#SBATCH -o directorioSalida/outputMpi2048_T16.txt
#SBATCH -e directorioSalida/erroresMpi2048_T16.txt

mpirun eje 2048
