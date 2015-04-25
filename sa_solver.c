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
void initialize(char puzzle[][SIZE]);
char isVld(char puzzle[][SIZE], int row, int col, char num);
void elimination(char numbers[], char puzzle[][SIZE], char aux[][SIZE][SIZE + 2], int row, int col, char num);
void printA(char puzzle[][SIZE][SIZE+2]);
void printPuzzle(char puzzle[][SIZE]);

struct coor{ char x, y;};

char aux[SIZE][SIZE][SIZE + 2];
struct coor map[MAX_SWAP][2];
struct coor swapCandidate[SIZE*SIZE][2];
struct coor sortedIndexes[SIZE+1][SIZE*SIZE/2];

int anneal(char puzzle[][SIZE], double stoppingTemp, double rate, double temperature) {
   char best[SIZE][SIZE], neighbor[SIZE][SIZE];
   int dlta, costB, cost;
   initialize(puzzle);
   //printPuzzle(puzzle);
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
   printPuzzle(puzzle);
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
   map[0][0].x = -1;
   for (i = 0; i < swapCount; i++) {
      do {
         row1 = (rand() % SIZE);
         col1 = (rand() % SIZE);
      } while (aux[row1][col1][SIZE+1]);
      do {
         row2 = (SIZE==16) ? row1:(rand() % SIZE);
         col2 = (rand() % SIZE);
      } while (aux[row2][col2][SIZE+1] || (col1 == col2 && row1==row2));
      // || (!aux[row1][col1][puzzle[row2][col2]] && !aux[row2][col2][puzzle[row1][col1]])
      neighbor[row2][col2] = puzzle[row1][col1];
      neighbor[row1][col1] = puzzle[row2][col2];
      map[i][0].x = row1; map[i][0].y = col1;
      map[i][1].x = row2; map[i][1].y = col2;
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
                     x=0; y=0;
                     break;
                  }
               }
            }
            else if (aux[x][y][0] > 1) {
               sortedIndexes[aux[x][y][0]][counters[aux[x][y][0]]].x=x;
               sortedIndexes[aux[x][y][0]][counters[aux[x][y][0]]++].y=y;
            }
         }
      }
   }
   if (SIZE==16) {
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
               coorY = (rand() % SIZE);
               for (y=0; y<SIZE; y++) {
                  if (!puzzle[x][coorY] && aux[x][y][num]){
                     puzzle[x][coorY] = num;
                     numbers[num]++;
                     found=1;
                     break;
                  }
                  coorY = (coorY+1)%SIZE;
               }
               if (!found) {
                  for (y=0; y<SIZE; y++) {
                     if (!puzzle[x][y]) {
                        puzzle[x][y] = num;
                        numbers[num]++;
                        break;
                     }
                  }
               }
            }
         }
      }
   }else{
      for (x = 1; x <= SIZE; x++) {
         while (numbers[x] < SIZE) {
            coorX = (rand() % SIZE), coorY = (rand() % SIZE);
            if (!puzzle[coorX][coorY] && (aux[coorX][coorY][x] || !((rand() % SIZE)/N))) {
               puzzle[coorX][coorY] = x;
               numbers[x]++;
            }
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
                  break;
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

void printPuzzle(char puzzle[][SIZE]){
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

int main() {
   int found=0;
   clock_t finish, start;
   char puzzle9[9][9] = { {0, 0, 0, 0, 0, 0, 0, 0, 0},
      {0, 0, 3, 6, 0, 9, 7, 0, 0},
      {0, 6, 0, 0, 2, 0, 0, 8, 0},
      {0, 9, 0, 4, 0, 3, 0, 6, 0},
      {0, 0, 4, 0, 6, 0, 1, 0, 0},
      {0, 5, 0, 1, 0, 8, 0, 7, 0},
      {0, 7, 0, 0, 3, 0, 0, 5, 0},
      {0, 0, 1, 5, 0, 4, 2, 0, 0},
      {0, 0, 0, 0, 0, 0, 0, 0, 0}};
   
   char puzzle16[16][16] = {{0, 0, 4, 0, 0, 0, 0, 8, 0, 1, 9, 15, 0, 0, 14, 16},
      {0, 12, 0, 0, 9, 0, 0, 5, 2, 0, 7, 0, 0, 13, 8, 0},
      {0, 0, 0, 16, 15, 2, 0, 0, 0, 0, 0, 0, 3, 10, 0, 11},
      {15, 0, 0, 14, 0, 0, 6, 0, 0, 0, 0, 0, 0, 0, 0, 12},
      {11, 0, 0, 0, 0, 0, 14, 0, 8, 6, 0, 3, 0, 0, 9, 0},
      {0, 2, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 16, 0},
      {0, 0, 16, 0, 4, 0, 9, 0, 0, 11, 0, 0, 10, 1, 12, 5},
      {0, 0, 0, 8, 11, 13, 0, 2, 0, 15, 0, 10, 0, 14, 0, 0},
      {12, 14, 0, 0, 0, 3, 0, 0, 0, 0, 0, 0, 0, 0, 7, 4},
      {7, 0, 9, 0, 8, 6, 16, 0, 15, 0, 12, 13, 0, 2, 0, 0},
      {0, 3, 2, 6, 0, 0, 1, 10, 0, 0, 8, 0, 0, 0, 0, 0},
      {0, 0, 10, 0, 0, 0, 4, 13, 0, 9, 1, 0, 0, 0, 3, 0},
      {0, 5, 0, 0, 0, 0, 0, 0, 7, 0, 0, 0, 8, 0, 15, 0},
      {0, 0, 14, 0, 3, 0, 0, 0, 9, 12, 15, 11, 13, 0, 0, 0},
      {0, 0, 13, 11, 0, 0, 0, 0, 0, 0, 5, 14, 0, 9, 0, 2},
      {3, 0, 7, 0, 0, 0, 0, 0, 0, 0, 0, 4, 5, 6, 0, 0}};
   srand(time(NULL));
   start = clock();
   if(SIZE==16){
      if (anneal(puzzle16, 0.000001, 0.00001, 1000)) { finish = clock();printPuzzle(puzzle16);
      } else{ finish = clock(); printf("\n\nNO SOLUTION\n\n");}
   } else {
      //if (anneal(puzzle9, 0.000000001, 0.000001, 100)) { finish = clock();printPuzzle(puzzle9);
      //} else{ finish = clock(); printf("\n\nNO SOLUTION\n\n");}
   }
   printf("StartTime:%ld, EndTime:%ld, ExecutionTime:%f\n", start, finish, (double)(finish - start) / CLOCKS_PER_SEC);
}

/*
 2    7	4	3	10	12	13	8	11	1	9	15	6	5	14	16
 1    12	6	10	9	11	3	5	2	14	7	16	4	13	8	15
 5    9	8	16	15	2	7	14	6	4	13	12	3	10	1	11
 15	13	11	14	16	1	6	4	5	10	3	8	9	7	2	12
 11	10	1	12	5	16	14	7	8	6	4	3	2	15	9	13
 13	2	3	7	6	10	15	1	12	5	14	9	11	4	16	8
 14	6	16	15	4	8	9	3	13	11	2	7	10	1	12	5
 9    4	5	8	11	13	12	2	1	15	16	10	7	14	6	3
 12	14	15	13	2	3	5	9	10	16	11	6	1	8	7	4
 7    1	9	4	8	6	16	11	15	3	12	13	14	2	5	10
 16	3	2	6	12	14	1	10	4	7	8	5	15	11	13	9
 8    11	10	5	7	15	4	13	14	9	1	2	16	12	3	6
 10	5	12	9	13	4	11	16	7	2	6	1	8	3	15	14
 4    8	14	1	3	5	2	6	9	12	15	11	13	16	10	7
 6    16	13	11	1	7	10	15	3	8	5	14	12	9	4	2
 3    15	7	2	14	9	8	12	16	13	10	4	5	6	11	1
 */