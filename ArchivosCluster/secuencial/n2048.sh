#!/bin/bash
#SBATCH -N 1
#SBATCH --exclusive
#SBATCH -o directorioSalida/outputSeq2048.txt
#SBATCH -e directorioSalida/erroresSeq2048.txt
./eje 2048

