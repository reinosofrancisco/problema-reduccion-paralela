#!/bin/bash
#SBATCH -N 1
#SBATCH --exclusive
#SBATCH -o directorioSalida/outputSeq1024.txt
#SBATCH -e directorioSalida/erroresSeq1024.txt
./eje 1024

