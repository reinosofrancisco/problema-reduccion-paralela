#!/bin/bash
#SBATCH -N 2
#SBATCH --exclusive
#SBATCH --tasks-per-node=8
#SBATCH -o directorioSalida/outputMpi1024_T16.txt
#SBATCH -e directorioSalida/erroresMpi1024_T16.txt

mpirun eje 1024
