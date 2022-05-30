#!/bin/bash
#SBATCH -N 2
#SBATCH --exclusive
#SBATCH --tasks-per-node=2
#SBATCH -o directorioSalida/outputMpi512_T4.txt
#SBATCH -e directorioSalida/erroresMpi512_T4.txt

mpirun eje 512
