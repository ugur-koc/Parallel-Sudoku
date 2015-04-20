#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <math.h>

#define SIZE 16
#define N 4
#define MAX_SWAP 1

void transform(char puzzle[][SIZE], char neighbor[][SIZE], int swapCount);
int delta(char puzzle[][SIZE], char neighbor[][SIZE]);
int costF(char puzzle[][SIZE]);
void accept(char src[][SIZE], char dest[][SIZE]);
void copy(char src[][SIZE], char dest[][SIZE]);
void printP(char puzzle[][SIZE]);
void initialize(char puzzle[][SIZE]);
char isVld(char puzzle[][SIZE], int row, int col, char num);
void elimination(char numbers[], char puzzle[][SIZE], char aux[][SIZE][SIZE + 2], int row, int col, char num);
struct coor{ char x, y; };
void printA(char puzzle[][SIZE][SIZE+2]);

char aux[SIZE][SIZE][SIZE + 2];
struct coor map[MAX_SWAP][2];

int anneal(char puzzle[][SIZE]) {
   char best[SIZE][SIZE], neighbor[SIZE][SIZE];
   int dlta, costB, cost;
   double stoppingTemp = 0.000001, rate = 0.00001, temperature = 1000;
   initialize(puzzle);
   copy(puzzle, best);
   cost = costF(puzzle);
   costB = cost;
   printf("Cost:%d\n",costB);
   //printP(puzzle);
   while (cost > 0 && stoppingTemp < temperature) {
      transform(puzzle, neighbor, MAX_SWAP);
      dlta = delta(puzzle,neighbor);
      if (dlta < 0 || (((double)(rand() % 1000) / 1000.0) <= exp(-dlta / temperature))) {
           // printf("accept\n");
         accept(neighbor, puzzle);
         cost += dlta;;
         if (costB > cost){
            costB = cost;
            copy(puzzle, best);
            //printP(puzzle);
         }
      }
      //else printf("reject\n");
      temperature -= rate * temperature;
   }
   copy(best, puzzle);
   printf("Cost:%d\n",costB);
   //printP(puzzle);
   return costB ? 0 : 1;
}

void accept(char src[][SIZE], char dest[][SIZE]) {
   int i=0,row1,col1,row2,col2;
   while (map[i][0].x != -1) {
      dest[map[i][0].x][map[i][0].y] = src[map[i][0].x][map[i][0].y];
      dest[map[i][1].x][map[i][1].y] = src[map[i][1].x][map[i][1].y];
      i++;
   }
}

void transform(char puzzle[][SIZE], char neighbor[][SIZE], int swapCount) {
   int i, row1, col1, row2, col2;
   copy(puzzle, neighbor);
   for (i = 0; i < swapCount; i++) {
      do {
         row1 = (rand() % SIZE);
         col1 = (rand() % SIZE);
      } while (aux[row1][col1][SIZE]);
      do {
         row2 = (rand() % SIZE);
         col2 = (rand() % SIZE);
      } while (aux[row2][col2][SIZE] || (row1 == row2 && col1 == col2) || puzzle[row1][col1] == puzzle[row2][col2] || !(aux[row1][col1][puzzle[row2][col2]-1] || aux[row2][col2][puzzle[row1][col1]-1]));
      //printf("r1:%d, c1:%d, r2:%d, c2:%d, puzzle[r1][c1]:%d, puzzl2[r2][c2]:%d ",row1,col1,row2,col2,puzzle[row1][col1], puzzle[row2][col2]);
      neighbor[row2][col2] = puzzle[row1][col1];
      neighbor[row1][col1] = puzzle[row2][col2];
      map[i][0].x = row1;
      map[i][0].y = col1;
      map[i][1].x = row2;
      map[i][1].y = col2;
      map[i+1][0].x = -1;
   }
}

int costF(char puzzle[][SIZE]) {
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
   return cost/2;
}

int delta(char puzzle[][SIZE], char neighbor[][SIZE]) {
   int i, j, k = 0 , row, col, colS, rowS, numP, numN, cost=0;
   while (map[k][0].x != -1) {
      for (j=0; j<=1; j++) {
         row = map[k][j].x;
         col = map[k][j].y;
         colS = (col/N) * N;
         rowS = (row/N) * N;
         numP = puzzle[row][col];
         numN = neighbor[row][col];
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

void initialize(char puzzle[][SIZE]) {
   int x, y, coorX, coorY;
   char z, numbers[SIZE+1]={0}, num = 0;
   for (x = 0; x < SIZE; x++){
      for (y = 0; y < SIZE; y++){
         if (puzzle[x][y]){
            aux[x][y][SIZE] = SIZE + 1;
            numbers[puzzle[x][y]]++;
         } else {
            aux[x][y][SIZE] = 0; aux[x][y][SIZE + 1] = 0;
            for (z = 0; z < SIZE; z++){
               aux[x][y][z] = isVld(puzzle, x, y, z+1);
               aux[x][y][SIZE + 1] += aux[x][y][z];
            }
         }
      }
   }
   //printP(puzzle);
   //printA(aux);
   for (x = 0; x < SIZE; x++){
      for (y = 0; y < SIZE; y++){
         if (!puzzle[x][y] && aux[x][y][SIZE + 1] == 1){
            for (z = 0; z < SIZE; z++){
               if (aux[x][y][z]) {
                  //printf("x:%d, y:%d, z:%d, count:%d\n", x , y, z+1, aux[x][y][SIZE+1]);
                  //printP(puzzle);
                  elimination(numbers, puzzle, aux, x, y, z+1);
                  //printP(puzzle);
                  //printA(aux);
                  break;
               }
            }
         }
      }
   }
   //printP(puzzle);
   //printA(aux);
   for (x = 1; x <= SIZE; x++) {
      while (numbers[x] < SIZE) {
         coorX = (rand() % SIZE), coorY = (rand() % SIZE);
         if (!puzzle[coorX][coorY] && (aux[coorX][coorY][x-1] || !((rand() % SIZE)/N))) {
            //printf("x:%d, coorX:%d, coorY:%d, puzzle[coorX][coorY]:%d, aux[coorX][coorY][x-1]:%d\n",x, coorX,coorY,puzzle[coorX][coorY],aux[coorX][coorY][x-1]);
            puzzle[coorX][coorY] = x;
            numbers[x]++;
            //printP(puzzle);
            //printA(aux);
         }
      }
   }
}

void elimination(char numbers[], char puzzle[][SIZE], char aux[][SIZE][SIZE + 2], int row, int col, char num) {
   int i, j, rowStart = (row / N) * N, colStart = (col / N) * N;
   //printA(aux);
   //printP(puzzle);
   aux[row][col][SIZE] = SIZE + 1;
   aux[row][col][SIZE+1]=0;
   aux[row][col][num-1]=0;
   puzzle[row][col] = num;
   numbers[num]++;
   for (i = 0; i < SIZE; ++i){
      if (aux[row][i][num-1]){
         aux[row][i][num-1] = 0;
         //printf("Col i:%d, x:%d, y:%d, num:%d, count:%d\n", i, row , i, num, aux[row][i][SIZE+1]);
         if (--aux[row][i][SIZE+1] == 1){
            for (j=0; j<SIZE; j++) {
               if (aux[row][i][j]) {
                  //printf("elimination: row:%d, i:%d, j+1:%d\n", row, i, j+1);
                  elimination(numbers, puzzle, aux, row, i, j+1);
               }
            }
            
         }
      }
      if (aux[i][col][num-1]){
         aux[i][col][num-1] = 0;
         //printf("Row i:%d, x:%d, y:%d, num:%d, count:%d\n", i, i , col, num, aux[i][col][SIZE+1]);
         if (--aux[i][col][SIZE+1] == 1){
            for (j=0; j<SIZE; j++) {
               if (aux[i][col][j]) {
                 // printf("elimination: i:%d, col:%d, j+1:%d\n", i, col,j+1);
                  elimination(numbers, puzzle, aux, i, col, j+1);
                  break;
               }
            }
         }
      }
      if (aux[rowStart + (i % N)][colStart + (i / N)][num-1]){
         aux[rowStart + (i % N)][colStart + (i / N)][num-1] = 0;
         //printf("Sub i:%d, x:%d, y:%d, num:%d, count:%d\n", i, rowStart + (i % N) , colStart + (i / N), num, aux[rowStart + (i % N)][colStart + (i / N)][SIZE+1]);
         if (--aux[rowStart + (i % N)][colStart + (i / N)][SIZE+1] == 1){
            for (j=0; j<SIZE; j++) {
               if (aux[rowStart + (i % N)][colStart + (i / N)][j]) {
                  //printf("elimination: rowStart+(iN):%d, colStart+(i/N):%d, j+1:%d\n", rowStart + (i % N), colStart + (i / N),j+1);
                  elimination(numbers, puzzle, aux, rowStart + (i % N), colStart + (i / N), j+1);
                  break;
               }
            }
         }
      }
   }
}

void copy(char src[][SIZE], char dest[][SIZE]) {
   int i,j;
   for (i = 0; i < SIZE; i++)
   for (j = 0; j < SIZE; j++)
   dest[i][j] = src[i][j];
}

char isVld(char puzzle[][SIZE], int row, int col, char num) {
   int i, rowStart = (row / N) * N, colStart = (col / N) * N;
   for (i = 0; i < SIZE; ++i)
   if ((puzzle[row][i] == num) || (puzzle[i][col] == num) ||
       (puzzle[rowStart + (i % N)][colStart + (i / N)] == num)) return 0;
   return 1;
}

void printP(char puzzle[][SIZE]){
   int i,j;
   printf("\n+-----+-----+-----+\n");
   for(i = 1; i <= SIZE; ++i) {
      for(j = 1; j <= SIZE; ++j) printf("|%d", puzzle[i-1][j-1]);
      printf("|\n");
      if (i%N == 0) printf("+-----+-----+-----+\n");
   }
}

void printA(char puzzle[][SIZE][SIZE+2]){
   int i,j,k;
   printf("\n+-----+-----+-----+\n");
   for(i = 1; i <= SIZE; ++i) {
      for(j = 1; j <= SIZE; ++j){
         for (k=0; k<SIZE+2; k++) {
            printf("%d", puzzle[i-1][j-1][k]%SIZE);
         }
         printf("|");
      }
         printf("\n");
      if (i%N == 0) printf("+-----+-----+-----+\n");
   }
}