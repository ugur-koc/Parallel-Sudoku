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

int main() {
   test9();
   //test16('s');
   return 0;
}

void test9(){
   clock_t finish, start;
   char puzzle[SIZE][SIZE] = { {0, 0, 0, 0, 0, 0, 0, 0, 0},
      {0, 0, 3, 6, 0, 9, 7, 0, 0},
      {0, 6, 0, 0, 2, 0, 0, 8, 0},
      {0, 9, 0, 4, 0, 3, 0, 6, 0},
      {0, 0, 4, 0, 6, 0, 1, 0, 0},
      {0, 5, 0, 1, 0, 8, 0, 7, 0},
      {0, 7, 0, 0, 3, 0, 0, 5, 0},
      {0, 0, 1, 5, 0, 4, 2, 0, 0},
      {0, 0, 0, 0, 0, 0, 0, 0, 0}};
   
   start = clock();
   if (solve(puzzle, 0, 0)) { printPuzzle(puzzle);
   } else printf("\n\nNO SOLUTION\n\n");
   finish = clock();
   printf("StartTime:%ld, EndTime:%ld, ExecutionTime:%f\n", start, finish, (double)(finish - start) / CLOCKS_PER_SEC);
}

/*
 void test16(char alg){
 int found=0;
 clock_t finish, start;
 char puzzle[16][16] = {{4, 0, 6, 0, 0, 0, 0, 12, 0, 0, 0, 0, 11, 0, 0, 14},
 {15, 0, 0, 0, 0, 0, 0, 10, 0, 0, 14, 0, 13, 0, 0, 0},
 {0, 7, 0, 0, 0, 0, 2, 1, 0, 0, 10, 0, 0, 0, 12, 0},
 {16, 3, 14, 0, 9, 0, 0, 5, 0, 2, 0, 0, 0, 15, 0, 0},
 {0, 14, 0, 0, 6, 0, 0, 4, 0, 0, 0, 13, 8, 0, 0, 0},
 {0, 4, 0, 10, 0, 0, 0, 11, 0, 16, 15, 12, 7, 0, 0, 0},
 {0, 0, 12, 0, 0, 0, 15, 13, 0, 0, 0, 0, 6, 14, 0, 0},
 {13, 8, 5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 0, 10, 0},
 {12, 0, 0, 13, 15, 11, 9, 0, 0, 14, 0, 0, 0, 0, 0, 8},
 {5, 9, 0, 0, 0, 0, 0, 0, 0, 0, 8, 0, 14, 0, 0, 7},
 {0, 0, 0, 0, 0, 0, 0, 0, 15, 3, 0, 0, 0, 12, 6, 9},
 {0, 0, 8, 0, 14, 0, 1, 6, 0, 0, 16, 5, 0, 0, 11, 13},
 {6, 11, 0, 4, 0, 0, 5, 0, 7, 0, 1, 0, 0, 2, 0, 0},
 {7, 0, 0, 14, 12, 0, 0, 0, 13, 10, 0, 0, 0, 6, 0, 0},
 {0, 0, 0, 0, 1, 0, 13, 0, 9, 0, 3, 6, 0, 0, 15, 0},
 {0, 0, 0, 0, 10, 0, 0, 3, 0, 0, 0, 4, 0, 1, 0, 0}};
 start = clock();
 if (alg == 's'){
 found = anneal(puzzle);
 }else if (alg == 'd') {
 found = solve(puzzle, 0, 0);
 }
 finish = clock();
 
 if (found) { printPuzzle(puzzle);
 } else printf("\n\nNO SOLUTION\n\n");
 printf("StartTime:%ld, EndTime:%ld, ExecutionTime:%f\n", start, finish, (double)(finish - start) / CLOCKS_PER_SEC);
 }
 */

/*
 {0,0,2,0,0,0,3,6,0},
 {3,0,4,0,2,0,0,0,1},
 {0,0,0,0,6,0,0,0,5},
 {1,0,0,3,0,0,9,0,0},
 {0,9,0,0,8,6,5,0,0},
 {0,2,0,0,0,5,0,0,3},
 {8,0,0,7,0,0,0,0,0},
 {0,0,0,0,9,0,0,0,2},
 {0,1,0,0,0,0,0,0,8}};
 
 {{0, 0, 0, 0, 0, 0, 0, 0, 0},
 {0, 0, 3, 6, 0, 9, 7, 0, 0},
 {0, 6, 0, 0, 2, 0, 0, 8, 0},
 {0, 9, 0, 4, 0, 3, 0, 6, 0},
 {0, 0, 4, 0, 6, 0, 1, 0, 0},
 {0, 5, 0, 1, 0, 8, 0, 7, 0},
 {0, 7, 0, 0, 3, 0, 0, 5, 0},
 {0, 0, 1, 5, 0, 4, 2, 0, 0},
 {0, 0, 0, 0, 0, 0, 0, 0, 0}};
 
 {4, 0, 6, 0, 0, 0, 0, 12, 0, 0, 0, 0, 11, 0, 0, 14},
 {15, 0, 0, 0, 0, 0, 0, 10, 0, 0, 14, 0, 13, 0, 0, 0},
 {0, 7, 0, 0, 0, 0, 2, 1, 0, 0, 10, 0, 0, 0, 12, 0},
 {16, 3, 14, 0, 9, 0, 0, 5, 0, 2, 0, 0, 0, 15, 0, 0},
 {0, 14, 0, 0, 6, 0, 0, 4, 0, 0, 0, 13, 8, 0, 0, 0},
 {0, 4, 0, 10, 0, 0, 0, 11, 0, 16, 15, 12, 7, 0, 0, 0},
 {0, 0, 12, 0, 0, 0, 15, 13, 0, 0, 0, 0, 6, 14, 0, 0},
 {13, 8, 5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 0, 10, 0},
 {12, 0, 0, 13, 15, 11, 9, 0, 0, 14, 0, 0, 0, 0, 0, 8},
 {5, 9, 0, 0, 0, 0, 0, 0, 0, 0, 8, 0, 14, 0, 0, 7},
 {0, 0, 0, 0, 0, 0, 0, 0, 15, 3, 0, 0, 0, 12, 6, 9},
 {0, 0, 8, 0, 14, 0, 1, 6, 0, 0, 16, 5, 0, 0, 11, 13},
 {6, 11, 0, 4, 0, 0, 5, 0, 7, 0, 1, 0, 0, 2, 0, 0},
 {7, 0, 0, 14, 12, 0, 0, 0, 13, 10, 0, 0, 0, 6, 0, 0},
 {0, 0, 0, 0, 1, 0, 13, 0, 9, 0, 3, 6, 0, 0, 15, 0},
 {0, 0, 0, 0, 10, 0, 0, 3, 0, 0, 0, 4, 0, 1, 0, 0}}
 
 */