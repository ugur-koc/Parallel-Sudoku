#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <math.h>

#define SIZE 9
#define MAX_SWAP 10

struct Transition {
   int firstCell[2], secondCell[2];
};

int fix[SIZE][SIZE];
struct Transition map[MAX_SWAP];

void copy(int src[][SIZE], int dest[][SIZE]) {
   for (int i = 0; i < SIZE; i++)
   for (int j = 0; j < SIZE; j++)
   dest[i][j] = src[i][j];
}

void initialize(int puzzle[][SIZE]) {
   int x, coorX, coorY, numbers[SIZE+1]={0};
   for (x = 0; x < SIZE; x++){
      for (int y = 0; y < SIZE; y++){
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

//this function is time consuming, can be improved most probably
int costF(int puzzle[][SIZE]) {
   int temp[SIZE][SIZE];
   copy(puzzle,temp);
   if(map[0].firstCell[0]){
      temp[map[0].firstCell[0]][map[0].firstCell[1]] = puzzle[map[0].secondCell[0]][map[0].secondCell[1]];
      temp[map[0].secondCell[0]][map[0].secondCell[1]] = puzzle[map[0].firstCell[0]][map[0].firstCell[1]];
   }
   
   int colStart, rowStart, num, cost = 0;
   for (int row = 0; row < SIZE; row++) {
      for (int col = 0; col < SIZE; col++) {
         num = temp[row][col]; colStart = (col/3) * 3; rowStart = (row/3) * 3;
         for(int i = 0; i<SIZE; ++i) {
            if (((temp[row][i] == num) || (temp[i][col] == num)
                 || (temp[rowStart + (i%3)][colStart + (i/3)] == num))
                && (row != i || col != i) && (row != rowStart + (i/3) || col != colStart + (i/3)))
            cost++;
         }
      }
   }
   return cost;
}

void printPuzzle(int puzzle[][SIZE]){
   for(int i = 1; i <= SIZE; ++i) {
      for(int j = 1; j <= SIZE; ++j)
      printf("%d,", puzzle[i-1][j-1]);
   }
   printf("\n");
}
/*
 printf("\n+-----+-----+-----+\n");
 for(int i = 1; i <= SIZE; ++i) {
 for(int j = 1; j <= SIZE; ++j) printf("|%d", puzzle[i-1][j-1]);
 printf("|\n");
 if (i%3 == 0) printf("+-----+-----+-----+\n");
 }
 
 */

void transform(int puzzle[][SIZE]) {
   do {
      map[0].firstCell[0] = (rand() % SIZE);
      map[0].firstCell[1] = (rand() % SIZE);
   } while (fix[map[0].firstCell[0]][map[0].firstCell[1]]);
   do {
      map[0].secondCell[0] = (rand() % SIZE);
      map[0].secondCell[1] = (rand() % SIZE);
   } while (fix[map[0].secondCell[0]][map[0].secondCell[1]] &&
            !(map[0].firstCell[0] == map[0].secondCell[0] && map[0].firstCell[1] == map[0].secondCell[1]));
}

void accept(int puzzle[][SIZE]) {
   int tempNum, t = 0;
   while (!map[t].firstCell[0]) {
      tempNum = puzzle[map[t].firstCell[0]][map[t].firstCell[1]];
      puzzle[map[t].firstCell[0]][map[t].firstCell[1]] = puzzle[map[t].secondCell[0]][map[t].secondCell[1]];
      puzzle[map[t].secondCell[0]][map[t].secondCell[1]] = tempNum;
      map[t++].firstCell[0] = 0;
   }
}

int isAvailable(int puzzle[][SIZE], int row, int col, int num) {
   int rowStart = (row/3) * 3, colStart = (col/3) * 3;
   for(int i = 0; i<SIZE; ++i)
   if ((puzzle[row][i] == num) || (puzzle[i][col] == num)
       || (puzzle[rowStart + (i%3)][colStart + (i/3)] == num)) return 0;
   return 1;
}

int anneal(int puzzle[][SIZE]) {
   int delta, costN, best[SIZE][SIZE], cost = costF(puzzle);
   double stoppingTemp = 0.001, rate = 0.01, temperature = 1; //startingTemp = 1
   copy(puzzle, best);
   initialize(puzzle);
   cost = costF(puzzle);
   while (cost > 0 && stoppingTemp < temperature) {
      printPuzzle(puzzle);
      transform(puzzle);
      costN = costF(puzzle);
      delta = costN - cost;
      if (delta < 0 || ((rand() % 10000) / 10000.0) <= exp(-delta / temperature) ) {
         accept(puzzle);
         cost = costN;
         if (costF(best) > costN) copy(puzzle, best);
      }
      temperature -= rate * temperature;
   }
   copy(best, puzzle);
   return costF(best);
}

int main() {
   int i, j, puzzle[SIZE][SIZE] = {
      {0, 0, 0, 0, 0, 0, 0, 0, 0},
      {0, 0, 3, 6, 0, 9, 7, 0, 0},
      {0, 6, 0, 0, 2, 0, 0, 8, 0},
      {0, 9, 0, 4, 0, 3, 0, 6, 0},
      {0, 0, 4, 0, 6, 0, 1, 0, 0},
      {0, 5, 0, 1, 0, 8, 0, 7, 0},
      {0, 7, 0, 0, 3, 0, 0, 5, 0},
      {0, 0, 1, 5, 0, 4, 2, 0, 0},
      {0, 0, 0, 0, 0, 0, 0, 0, 0}};
   
   srand(time(NULL));
   if(anneal(puzzle)) { printPuzzle(puzzle);
   } else printf("\n\nNO SOLUTION\n\n");
   
   return 0;
}