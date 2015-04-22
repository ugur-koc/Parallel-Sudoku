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
void printA(char puzzle[][SIZE][SIZE+2]);

struct coor{ char x, y;};

char aux[SIZE][SIZE][SIZE + 2];
struct coor map[MAX_SWAP][2];

int anneal(char puzzle[][SIZE]) {
   char best[SIZE][SIZE], neighbor[SIZE][SIZE];
   int dlta, costB, cost;
   double stoppingTemp = 0.0000001, rate = 0.000001, temperature = 1000000;
   initialize(puzzle);
   copy(puzzle, best);
   cost = costF(puzzle);
   costB = cost;
   printf("Cost:%d\n",costB);
   while (cost > 0 && stoppingTemp < temperature) {
      transform(puzzle, neighbor, MAX_SWAP);
      dlta = delta(puzzle,neighbor);
      if (dlta < 0 || (((double)(rand() % 1000) / 1000.0) <= exp(-dlta / temperature))) {
         accept(neighbor, puzzle);
         cost += dlta;;
         if (costB > cost){
            costB = cost;
            copy(puzzle, best);
         }
      }
      temperature -= rate * temperature;
   }
   copy(best, puzzle);
   printf("Cost:%d\n",costB);
   printP(puzzle);
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
   int i, row1, row2, col1, col2;
   copy(puzzle, neighbor);
   for (i = 0; i < swapCount; i++) {
      do {
         row1 = (rand() % SIZE);
         col1 = (rand() % SIZE);
      } while (aux[row1][col1][SIZE+1]);
      do {
         row2 = row1;
         col2 = (rand() % SIZE);
      } while (aux[row2][col2][SIZE+1] || (col1 == col2 && row1==row2) || (!aux[row1][col1][puzzle[row2][col2]] && !aux[row2][col2][puzzle[row1][col1]]));
      //  || (!aux[row1][col1][puzzle[row1][col2]] && !aux[row1][col2][puzzle[row1][col1]])
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
            if (puzzle[row][i] == num)
            cost++;
            if (puzzle[i][col] == num)
            cost++;
            if (puzzle[rowStart + (i%N)][colStart + (i/N)] == num)
            cost++;
         }
         cost -=3;
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
            if (puzzle[row][i] == numP)
            cost--;
            if (puzzle[i][col] == numP)
            cost--;
            if (puzzle[rowS + (i%N)][colS + (i/N)] == numP)
            cost--;
            if (neighbor[row][i] == numN)
            cost++;
            if (neighbor[i][col] == numN)
            cost++;
            if (neighbor[rowS + (i%N)][colS + (i/N)] == numN)
            cost++;
         }
      }
      k++;
   }
   return cost;
}

void initialize(char puzzle[][SIZE]) {
   int x, y, coorX, coorY;
   char z, numbers[SIZE+1]={0}, num = 0, counters[SIZE]={0},found;
   //struct coor sortedIndexes[SIZE+1][SIZE*SIZE];
   for (x = 0; x < SIZE; x++){
      for (y = 0; y < SIZE; y++){
         if (puzzle[x][y]){
            aux[x][y][SIZE+1] = SIZE + 1;
            numbers[puzzle[x][y]]++;
         } else {
            aux[x][y][SIZE+1] = 0; aux[x][y][0] = 0;
            for (z = 1; z <= SIZE; z++){
               aux[x][y][z] = isVld(puzzle, x, y, z);
               aux[x][y][0] += aux[x][y][z];
            }
         }
      }
   }

   for (x = 0; x < SIZE; x++) {
      for (y = 0; y < SIZE; y++) {
         if (!puzzle[x][y]) {
            if (aux[x][y][0] == 1){
               for (z = 1; z <= SIZE; z++) {
                  if (aux[x][y][z]) {
                     elimination(numbers, puzzle, aux, x, y, z);
                     break;
                  }
               }
            }
            //else if (aux[x][y][0] > 1) {
              // sortedIndexes[aux[x][y][0]][counters[aux[x][y][0]]].x=x;
            //   sortedIndexes[aux[x][y][0]][counters[aux[x][y][0]]++].y=y;
            //}
         }
      }
   }
   
   for (num=1; num<=SIZE; num++) {
      for (x = 0; x < SIZE; x++) {
         found=0;
         for (y = 0; y < SIZE; y++) {
            if (puzzle[x][y]==num) {
               found=1;
               break;
            }
         }
         if (!found) {
            do {
               coorY = (rand() % SIZE);
            } while (puzzle[x][coorY]); // || !aux[x][y][num]
            puzzle[x][coorY] = num;
            numbers[num]++;
         }
      }
   }
}


void elimination(char numbers[], char puzzle[][SIZE], char aux[][SIZE][SIZE + 2], int row, int col, char num) {
   int i, j, rowStart = (row / N) * N, colStart = (col / N) * N;
   aux[row][col][SIZE+1] = SIZE + 1;
   aux[row][col][0]=0;
   aux[row][col][num]=0;
   puzzle[row][col] = num;
   numbers[num]++;
   for (i = 0; i < SIZE; ++i){
      if (aux[row][i][num]){
         aux[row][i][num] = 0;
         if (--aux[row][i][0] == 1){
            for (j=1; j<=SIZE; j++) {
               if (aux[row][i][j]) {
                  elimination(numbers, puzzle, aux, row, i, j);
               }
            }
         }
      }
      if (aux[i][col][num]){
         aux[i][col][num] = 0;
         if (--aux[i][col][0] == 1){
            for (j=1; j<=SIZE; j++) {
               if (aux[i][col][j]) {
                  elimination(numbers, puzzle, aux, i, col, j);
                  break;
               }
            }
         }
      }
      if (aux[rowStart + (i % N)][colStart + (i / N)][num]){
         aux[rowStart + (i % N)][colStart + (i / N)][num] = 0;
         if (--aux[rowStart + (i % N)][colStart + (i / N)][0] == 1){
            for (j=1; j<=SIZE; j++) {
               if (aux[rowStart + (i % N)][colStart + (i / N)][j]) {
                  elimination(numbers, puzzle, aux, rowStart + (i % N), colStart + (i / N), j);
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
      for(j = 1; j <= SIZE; ++j) printf("|%2d", puzzle[i-1][j-1]);
      printf("|\n");
      if (i%N == 0) printf("+-----+-----+-----+\n");
   }
}

void printA(char puzzle[][SIZE][SIZE+2]){
   int i,j,k;
   printf("\n+-----+-----+-----+\n");
   for(i = 1; i <= SIZE; ++i) {
      for(j = 1; j <= SIZE; ++j){
         for (k=0; k<SIZE+2; k++) printf("%d", puzzle[i-1][j-1][k]%SIZE);
         printf("|");
      }
      printf("\n");
      if (i%N == 0) printf("+-----+-----+-----+\n");
   }
}