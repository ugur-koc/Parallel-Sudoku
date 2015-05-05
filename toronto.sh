#!/bin/bash
#SBATCH --ntasks=1
#SBATCH -t 00:10:00

. ~/.profile

module load gcc

#Sudoku_SerialSolver Sudoku_ParallelSolver Sudoku_ParallelSolver_EhsanLocking Sudoku_ParallelSolver_Signalling Sudoku_ParallelSolver_locks Sudoku_ParallelSolver_msgPassing Sudoku_ParallelSolver_msgPassing2

for element in Sudoku_SerialSolver Sudoku_ParallelSolver Sudoku_ParallelSolver_EhsanLocking Sudoku_ParallelSolver_Signalling Sudoku_ParallelSolver_locks Sudoku_ParallelSolver_msgPassing Sudoku_ParallelSolver_msgPassing2
do
    for threadNum in 4 8
    do
	a=0;
	while [ "$a" -lt 1 ]
	do
	    cd /lustre/cmsc-nsx/SudokuSource/"$element" && make clean && make NUM_THREADS=$threadNum && ./solver.exe ../Unsolved/evil16-5.txt
	    echo "$element :run=$a, ThreadNumber=$threadNum"
	    echo "******************************************";
	    a=`expr $a + 1`;
	done
    done
done

