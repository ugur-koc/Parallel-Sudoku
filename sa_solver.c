#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <math.h>

#define SIZE 9
#define N 3
#define MAX_SWAP 10

int fix[SIZE][SIZE];

void copy(int src[][SIZE], int dest[][SIZE]) {
  int i,j;
   for (i = 0; i < SIZE; i++)
   for (j = 0; j < SIZE; j++)
   dest[i][j] = src[i][j];
}

void printP(int puzzle[][SIZE]){
  int i,j;
  printf("\n+-----+-----+-----+\n");
   for(i = 1; i <= SIZE; ++i) {
      for(j = 1; j <= SIZE; ++j) printf("|%d", puzzle[i-1][j-1]);
      printf("|\n");
      if (i%N == 0) printf("+-----+-----+-----+\n");
   }
}


void initialize(int puzzle[][SIZE]) {
  int x, y, coorX, coorY, numbers[SIZE+1]={0};
   for (x = 0; x < SIZE; x++){
      for (y = 0; y < SIZE; y++){
         if (puzzle[x][y]){
            fix[x][y]=1;
            numbers[puzzle[x][y]]++;
         }else
         fix[x][y]=0;
      }
   }
   
   for (x = 1; x <= SIZE; x++) {
      while (numbers[x] < SIZE) {
         coorX = (rand() % SIZE), coorY = (rand() % SIZE);
         if (!puzzle[coorX][coorY]) {
            puzzle[coorX][coorY] = x;
            numbers[x]++;
         }
      }
   }
}

void transform(int puzzle[][SIZE], int neighbor[][SIZE], int swapCount) {
  int i, row1, col1, row2, col2;
   copy(puzzle, neighbor);
   for (i = 0; i < swapCount; i++) {
      do {
         row1 = (rand() % SIZE);
         col1 = (rand() % SIZE);
      } while (fix[row1][col1]);
      do {
         row2 = (rand() % SIZE);
         col2 = (rand() % SIZE);
      } while (fix[row2][col2] || (row1 == row2 && col1 == col2) || puzzle[row1][col1] == puzzle[row2][col2]);
      neighbor[row2][col2] = puzzle[row1][col1];
      neighbor[row1][col1] = puzzle[row2][col2];
   }
}

//this function is time consuming, can be improved most probably
int costF(int puzzle[][SIZE]) {
  int i, row, col, colStart, rowStart, num, cost = 0;
   for (row = 0; row < SIZE; row++) {
      for (col = 0; col < SIZE; col++) {
         num = puzzle[row][col];
         colStart = (col/N) * N;
         rowStart = (row/N) * N;
         for(i = 0; i < SIZE; i++) {
            if (puzzle[row][i] == num && col != i)
            cost++;
            if (puzzle[i][col] == num && row != i)
            cost++;
            if (puzzle[rowStart + (i%N)][colStart + (i/N)] == num && (row != rowStart + (i%N) || col != colStart + (i/N)))
            cost++;
         }
      }
   }
   return cost;
}

int anneal(int puzzle[][SIZE]) {
   int delta, costN, costB, best[SIZE][SIZE], neighbor[SIZE][SIZE], cost;
   double prob, stoppingTemp = 0.001, rate = 0.0001, temperature = 10; //startingTemp = 1
   initialize(puzzle);
   copy(puzzle, best);
   cost = costF(puzzle);
   costB = cost;
   while (cost > 0 && stoppingTemp < temperature) {
      transform(puzzle, neighbor, 1);
      costN = costF(neighbor);
      delta = costN - cost;
      printP(puzzle);
      printP(neighbor);
      printf("delta:%d, costP:%d, costN:%d", delta, cost, costN);
      if (delta < 0 || (((double)(rand() % 1000) / 1000.0) <= exp(-delta / temperature))) {
         copy(neighbor, puzzle);
         cost = costN;
         if (costB > costN){
            costB = costN;
            copy(puzzle, best);
         }
      }
      temperature -= rate * temperature;
   }
   copy(best, puzzle);
   return !costB ? 1: 0;
}
