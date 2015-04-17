#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <math.h>

#define SIZE 9
#define N 3
#define MAX_SWAP 10

struct coor{
   int x,y;
};

int fix[SIZE][SIZE];
struct coor map[MAX_SWAP][2];
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
      map[i][0].x = row1;
      map[i][0].y = col1;
      map[i][1].x = row2;
      map[i][1].y = col2;
      map[i+1][0].x = -1;
      printf("row1:%d, col1:%d, row2:%d, col2:%d\n", map[i][0].x, map[i][0].y, map[i][1].x, map[i][1].y);
   }
}

int delta(int puzzle[][SIZE], int neighbor[][SIZE]) {
   int i, j, k = 0 , row, col, colS, rowS, numP, numN, cost=0;
   printf("\n");
   while (map[k][0].x != -1) {
      for (j=0; j<=1; j++) {
         row = map[k][j].x;
         col = map[k][j].y;
         colS = (col/N) * N;
         rowS = (row/N) * N;
         numP = puzzle[row][col];
         numN = neighbor[row][col];
         printf("row:%d, col:%d, numP:%d, numN:%d\n", row, col, numP, numN);
         for(i = 0; i < SIZE; i++) {
            if (puzzle[row][i] == numP && col != i)
            cost--;
            if (puzzle[i][col] == numP && row != i)
            cost--;
            if (puzzle[rowS + (i%N)][colS + (i/N)] == numP && (row != rowS + (i%N) || col != colS + (i/N)))
            cost--;
            if (neighbor[row][i] == numN && col != i)
            cost++;
            if (neighbor[i][col] == numN && row != i)
            cost++;
            if (neighbor[rowS + (i%N)][colS + (i/N)] == numN && (row != rowS + (i%N) || col != colS + (i/N)))
            cost++;
         }
      }
      k++;
   }
   return cost;
}
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
   int dlta, costN, costB, best[SIZE][SIZE], neighbor[SIZE][SIZE], cost, local1, local2;
   double prob, stoppingTemp = 0.001, rate = 0.0001, temperature = 1; //startingTemp = 1
   initialize(puzzle);
   copy(puzzle, best);
   cost = costF(puzzle);
   costB = cost;
   while (cost > 0 && stoppingTemp < temperature) {
      transform(puzzle, neighbor, 1);
      dlta = delta(puzzle, neighbor);
      if (dlta < 0 || (((double)(rand() % 1000) / 1000.0) <= exp(-dlta / temperature))) {
         printf("delta:%d, cost:%d\n", dlta, cost);
         costN = costF(neighbor);
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
   return costB ? 0 : 1;
}
