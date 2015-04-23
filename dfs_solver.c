#include <stdio.h>

#define SIZE 9
#define N 3

int isValid(char puzzle[][SIZE], int row, int col, int num) {
  int i, rowStart = (row / N) * N, colStart = (col / N) * N;
   for (i = 0; i < SIZE; ++i)
   if ((puzzle[row][i] == num) || (puzzle[i][col] == num) ||
       (puzzle[rowStart + (i % N)][colStart + (i / N)] == num)) return 0;
   return 1;
}

int solve(char puzzle[][SIZE], int row, int col) {
   int i;
   if(row < SIZE && col < SIZE) {
      if (puzzle[row][col] != 0) {
         if ((col + 1) < SIZE) return solve(puzzle, row, col + 1);
         else if ((row + 1) < SIZE) return solve(puzzle, row + 1, 0);
         else return 1;
      } else {
         for (i = 0; i < SIZE; ++i) {
            if (isValid(puzzle, row, col, i + 1)) {
               puzzle[row][col] = i + 1;
               if ((col + 1) < SIZE) {
                  if (solve(puzzle, row, col + 1)) return 1;
                  else puzzle[row][col] = 0;
               } else if ((row + 1) < SIZE) {
                  if (solve(puzzle, row + 1, 0)) return 1;
                  else puzzle[row][col] = 0;
               } else return 1;
            }
         }
      }
      return 0;
   } else return 1;
}