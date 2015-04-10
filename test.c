#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <math.h>
#include "sa_solver.c"
#include "greedy_solver.c"

void printPuzzle(int puzzle[][SIZE]){
   printf("\n+-----+-----+-----+\n");
   for(int i = 1; i <= SIZE; ++i) {
      for(int j = 1; j <= SIZE; ++j) printf("|%d", puzzle[i-1][j-1]);
      printf("|\n");
      if (i%3 == 0) printf("+-----+-----+-----+\n");
   }
}

int main() {
   int puzzle[SIZE][SIZE] = {
      {6, 2, 0, 0, 0, 4, 5, 9, 0},
      {0, 7, 0, 9, 0, 5, 0, 0, 0},
      {5, 0, 3, 0, 0, 0, 0, 0, 0},
      {7, 0, 2, 0, 0, 0, 0, 3, 8},
      {3, 0, 9, 7, 8, 1, 4, 0, 5},
      {8, 5, 0, 0, 0, 0, 1, 0, 6},
      {0, 0, 0, 0, 0, 0, 8, 0, 2},
      {0, 0, 0, 1, 0, 3, 0, 4, 0},
      {0, 4, 6, 2, 0, 0, 0, 1, 3}};
   
   srand(time(NULL));
   clock_t finish, start = clock();
   if(!anneal(puzzle)) { printPuzzle(puzzle);
   } else printf("\n\nNO SOLUTION\n\n");
   finish = clock();
   float time_diff = (((float)finish - (float)start) / 1000000.0F ) * 1000;
   printf("Execution time:%.2f\n", time_diff);
   
   start = clock();
   if(fillSudoku(puzzle, 0, 0)) { printPuzzle(puzzle);
   } else printf("\n\nNO SOLUTION\n\n");
   finish = clock();
   time_diff = (((float)finish - (float)start) / 1000000.0F ) * 1000;
   printf("Execution time:%.2f\n", time_diff);
   
   return 0;
}

/*
 {{2, 4, 9, 8, 5, 7, 3, 1, 6},
 {8, 1, 3, 6, 4, 9, 7, 2, 5},
 {5, 6, 7, 3, 2, 1, 9, 8, 4},
 {1, 9, 2, 4, 7, 3, 5, 6, 8},
 {7, 8, 4, 2, 6, 5, 1, 3, 9},
 {3, 5, 6, 1, 9, 8, 4, 7, 2},
 {4, 7, 8, 9, 3, 2, 6, 5, 1},
 {6, 3, 1, 5, 8, 4, 2, 9, 7},
 {9, 2, 5, 7, 1, 6, 8, 4, 3}};
 
 {
 {0, 0, 0, 0, 0, 0, 0, 0, 0},
 {0, 0, 3, 6, 0, 9, 7, 0, 0},
 {0, 6, 0, 0, 2, 0, 0, 8, 0},
 {0, 9, 0, 4, 0, 3, 0, 6, 0},
 {0, 0, 4, 0, 6, 0, 1, 0, 0},
 {0, 5, 0, 1, 0, 8, 0, 7, 0},
 {0, 7, 0, 0, 3, 0, 0, 5, 0},
 {0, 0, 1, 5, 0, 4, 2, 0, 0},
 {0, 0, 0, 0, 0, 0, 0, 0, 0}};
 
 
 {0, 0, 0, 0, 0, 0, 0, 0, 0},
 {0, 0, 0, 0, 0, 0, 0, 0, 0},
 {0, 0, 0, 0, 0, 0, 0, 0, 0},
 {0, 0, 0, 0, 0, 0, 0, 0, 0},
 {0, 0, 0, 0, 0, 0, 0, 0, 0},
 {0, 0, 0, 0, 0, 0, 0, 0, 0},
 {0, 0, 0, 0, 0, 0, 0, 0, 0},
 {0, 0, 0, 0, 0, 0, 0, 0, 0},
 {0, 0, 0, 0, 0, 0, 0, 0, 0}};
 

 */