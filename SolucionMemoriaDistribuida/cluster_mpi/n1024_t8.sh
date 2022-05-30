#!/bin/bash
#SBATCH -N 2
#SBATCH --exclusive
#SBATCH --tasks-per-node=4
#SBATCH -o directorioSalida/outputMpi1024_T8.txt
#SBATCH -e directorioSalida/erroresMpi1024_T8.txt

mpirun eje 1024
