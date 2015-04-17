#!/bin/bash
#SBATCH --ntasks=1
#SBATCH -t 24:00:00

. ~/.profile

module load gcc

gcc -o solver test.c
./solver
