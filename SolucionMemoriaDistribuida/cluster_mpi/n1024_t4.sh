#!/bin/bash
#SBATCH -N 2
#SBATCH --exclusive
#SBATCH --tasks-per-node=2
#SBATCH -o directorioSalida/outputMpi1024_T4.txt
#SBATCH -e directorioSalida/erroresMpi1024_T4.txt

mpirun eje 1024
