#!/bin/bash
#SBATCH -N 2
#SBATCH --exclusive
#SBATCH --tasks-per-node=4
#SBATCH -o directorioSalida/outputMpi512_T8.txt
#SBATCH -e directorioSalida/erroresMpi512_T8.txt

mpirun eje 512
