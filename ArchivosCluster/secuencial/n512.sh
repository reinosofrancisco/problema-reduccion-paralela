#!/bin/bash
#SBATCH -N 1
#SBATCH --exclusive
#SBATCH -o directorioSalida/outputSeq512.txt
#SBATCH -e directorioSalida/erroresSeq512.txt
./eje 512

