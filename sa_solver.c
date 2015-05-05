#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <math.h>
#include <pthread.h>

#define SIZE 16
#define N 4
#define MAX_SWAP 1

void transform(int swapCount);
int delta();
int costF();
char isVld(int row, int col, char num);
void accept();
void copy(char dest[][SIZE]);
void copyTo(char dest[][SIZE]);
void initialize();
void checkForElimination(char numbers[]);
void checkForLoneRanger(char numbers[]);
void elimination(char numbers[], int row, int col, char num);
void printPuzzle(char best[SIZE][SIZE]);
void printAux();

struct coor{ char x, y;};

char aux[SIZE][SIZE][SIZE + 3];
struct coor map[MAX_SWAP][2];
//sortedIndexes[SIZE+1][SIZE*SIZE/2]
char best[SIZE][SIZE], neighbor[SIZE][SIZE], puzzle[SIZE][SIZE]= {{0, 0, 4, 0, 0, 0, 0, 8, 0, 1, 9, 15, 0, 0, 14, 16},
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

int anneal(double stoppingTemp, double rate, double temperature) {
   int dlta, costB, cost;
   pthread_t acceptCaller, copyCaller;
   initialize();
   copy(best);
   cost = costF();
   costB = cost;
   printf("Cost:%d\n",costB);
   while (cost > 0 && stoppingTemp < temperature) {
      transform(MAX_SWAP);
      dlta = delta();
      if (dlta < 0 || (((double)(rand() % 1000) / 1000.0) <= exp(-dlta / temperature))) {
         cost +=dlta;
         if (costB > cost){
            pthread_create (&acceptCaller, NULL, (void *) &accept, NULL);
            //accept();
            costB = cost;
            copyTo(best);
            pthread_join(acceptCaller, NULL);
         }else
            accept();
      }
      temperature -= rate * temperature;
   }
   printf("CostB:%d\n", costB);
   return costB ? 0 : 1;
}

void transform(int swapCount) {
   int i, j, row1, row2, col1, col2;
   copy(neighbor);
   map[0][0].x = -1;
   for (i = 0; i < swapCount; i++) {
      do {
         row1 = (rand() % SIZE);
         col1 = (rand() % SIZE);
         for (j = 0; (j<SIZE && !aux[row1][col1][SIZE + 2]); j++)
            col1 = (col1 + 1) % SIZE;
      } while (aux[row1][col1][SIZE + 1] || !aux[row1][col1][SIZE + 2]);
      do {
         row2 = row1;
         col2 = (rand() % SIZE);
      } while (aux[row2][col2][SIZE+1] || (col1 == col2 && row1==row2));
      neighbor[row2][col2] = puzzle[row1][col1];
      neighbor[row1][col1] = puzzle[row2][col2];
      map[i][0].x = row1; map[i][0].y = col1;
      map[i][1].x = row2; map[i][1].y = col2;
      map[i+1][0].x = -1;
   }
}

void accept() {
   int i=0, j, k, row, col, colS, rowS, numN,numP;
   while (map[i][0].x != -1) {
      for (j=0; j<=1; j++) {
         row = map[i][j].x; col = map[i][j].y;
         rowS = (row/N) * N; colS = (col/N) * N;
         numP = puzzle[row][col]; numN = neighbor[row][col];
         aux[row][col][SIZE + 2] = 0;
         for(k = 0; k < SIZE; k++) {
            if (neighbor[k][col] == numP)
               aux[k][col][SIZE + 2]--;
            if (neighbor[rowS + (k%N)][colS + (k/N)] == numP)
               aux[rowS + (k%N)][colS + (k/N)][SIZE + 2]--;
            if (puzzle[k][col] == numN) {
               aux[k][col][SIZE + 2]++;
               aux[row][col][SIZE + 2]++;
            }
            if (puzzle[rowS + (k%N)][colS + (k/N)] == numN) {
               aux[rowS + (k%N)][colS + (k/N)][SIZE + 2]++;
               aux[row][col][SIZE + 2]++;
            }
         }
         puzzle[row][col] = numN;
      }
      i++;
   }
}

int costF() {
   int i, row, col, colStart, rowStart, num, cost = 0;
   for (row = 0; row < SIZE; row++) {
      for (col = 0; col < SIZE; col++) {
         num = puzzle[row][col];
         colStart = (col/N) * N; rowStart = (row/N) * N;
         for(i = 0; i < SIZE; i++) {
            if (puzzle[i][col] == num && i!=row){
               aux[i][col][SIZE + 2]++;
               aux[row][col][SIZE + 2]++;
               cost++;
            }
            if (puzzle[rowStart + (i%N)][colStart + (i/N)] == num && (rowStart+(i%N)!=row || colStart+(i/N)!=col)){
               aux[rowStart + (i%N)][colStart + (i/N)][SIZE + 2]++;
               aux[row][col][SIZE + 2]++;
               cost++;
            }
         }
      }
   }
   for (row = 0; row < SIZE; row++) {
      for (col = 0; col < SIZE; col++) {
         aux[row][col][SIZE + 2] /=2;
      }
   }
   return cost/2;
}

int delta() {
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
            if (puzzle[i][col] == numP)
               cost--;
            if (puzzle[rowS + (i%N)][colS + (i/N)] == numP)
               cost--;
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

void initialize() {
   int x, y, coorY;
   char z, numbers[SIZE+1]={0}, num = 0, found;
   for (x = 0; x < SIZE; x++){
      for (y = 0; y < SIZE; y++){
         aux[x][y][SIZE+2]=0;
         if (puzzle[x][y]){
            aux[x][y][SIZE+1] = SIZE + 1;
            numbers[puzzle[x][y]]++;
         } else {
            aux[x][y][SIZE+1] = 0; aux[x][y][0] = 0;
            for (z = 1; z <= SIZE; z++){
               aux[x][y][z] = isVld(x, y, z);
               aux[x][y][0] += aux[x][y][z];
            }
         }
      }
   }
   checkForElimination(numbers);
   checkForLoneRanger(numbers);
   for (num = 1; num <= SIZE; num++) {
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
               if (!puzzle[x][coorY] && aux[x][coorY][num]){
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
   
}
void checkForElimination(char numbers[]) {
   int x,y,z;// counters[SIZE]={0};
   for (x = 0; x < SIZE; x++) {
      for (y = 0; y < SIZE; y++) {
         if (!puzzle[x][y]) {
            if (aux[x][y][0] == 1){
               for (z = 1; z <= SIZE; z++) {
                  if (aux[x][y][z]) {
                     elimination(numbers, x, y, z);
                     x=0; y=0;
                     break;
                  }
               }
            }
            //            else if (aux[x][y][0] > 1) {
            //               sortedIndexes[aux[x][y][0]][counters[aux[x][y][0]]].x=x;
            //               sortedIndexes[aux[x][y][0]][counters[aux[x][y][0]]++].y=y;
            //            }
         }
      }
   }
}

void checkForLoneRanger(char numbers[]) {
   int i,j,num, hAlone=0, hIndex, hFound, vAlone=0, vIndex,vFound;
   for (num=1; num<=SIZE; num++) {
      for (i = 0; i < SIZE; i++) {
         hFound=0; vFound=0; hAlone=0; vAlone=0;
         for (j = 0; j < SIZE; j++) {
            if (puzzle[i][j]==num) {
               hFound=1;
            } else if (aux[i][j][num] && !hFound) {
               hAlone++;
               hIndex=j;
            }
            if (puzzle[j][i]==num) {
               vFound=1;
            } else if (aux[j][i][num] && !vFound) {
               vAlone++;
               vIndex=j;
            }
         }
         if (!hFound && hAlone == 1) {
            puzzle[i][hIndex]=num;
            for (j=0; j<=SIZE; j++){
               aux[i][hIndex][j]=0;
               
               aux[i][j][0] = aux[i][j][0] - aux[i][j][num];
               aux[i][j][num]=0;
               
               aux[j][hIndex][0] = aux[j][hIndex][0] - aux[j][hIndex][num];
               aux[j][hIndex][num]=0;
               if (aux[i][j][0]==1)
                  elimination(numbers, i,j,num);
            }
            aux[i][hIndex][SIZE + 1]=SIZE + 1;
            numbers[num]++;
            //checkForLoneRanger(numbers);
         }
         if (!vFound && vAlone==1) {
            puzzle[vIndex][i]=num;
            for (j=0; j<=SIZE; j++) {
               aux[vIndex][i][j]=0;
               
               aux[j][i][0] = aux[j][i][0] - aux[j][i][num];
               aux[j][i][num]=0;
               
               aux[vIndex][j][0] = aux[vIndex][j][0] - aux[vIndex][j][num];
               aux[vIndex][j][num]=0;
               if (aux[j][i][0]==1)
                  elimination(numbers, j,i,num);
            }
            aux[vIndex][i][SIZE + 1]=SIZE + 1;
            numbers[num]++;
            //checkForLoneRanger(numbers);
         }
      }
   }
}

void elimination(char numbers[], int row, int col, char num) {
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
                  elimination(numbers, row, i, j);
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
                  elimination(numbers, i, col, j);
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
                  elimination(numbers, rowStart + (i % N), colStart + (i / N), j);
                  break;
               }
            }
         }
      }
   }
}

void copy(char dest[][SIZE]) {
   int i,j;
   for (i = 0; i < SIZE; i++)
      for (j = 0; j < SIZE; j++)
         dest[i][j] = puzzle[i][j];
}

void copyTo(char dest[][SIZE]) {
   int i,j;
   for (i = 0; i < SIZE; i++)
      for (j = 0; j < SIZE; j++)
         dest[i][j] = neighbor[i][j];
}

char isVld(int row, int col, char num) {
   int i, rowStart = (row / N) * N, colStart = (col / N) * N;
   for (i = 0; i < SIZE; ++i)
      if ((puzzle[row][i] == num) || (puzzle[i][col] == num) ||
          (puzzle[rowStart + (i % N)][colStart + (i / N)] == num)) return 0;
   return 1;
}

void printPuzzle(char puzzle[SIZE][SIZE]){
   int i,j;
   printf("\n+-----+-----+-----+\n");
   for(i = 1; i <= SIZE; ++i) {
      for(j = 1; j <= SIZE; ++j) printf("|%d", puzzle[i-1][j-1]);
      printf("|\n");
      if (i%N == 0) printf("+-----+-----+-----+\n");
   }
}

void printAux(){
   int i,j,k;
   for(i = 1; i <= SIZE; ++i) {
      for(j = 1; j <= SIZE; ++j){
         for(k = 1; k <= SIZE+3; ++k)
            printf("%d", aux[i-1][j-1][k-1]%SIZE);
         printf("|");
      }
      printf("\n");
      if (i%N == 0) printf("+-----+-----+-----+\n");
   }
}

int main(int argc, char *argv[]) {
   clock_t finish, start;
   double stoppingTemp=0.000000001, rate=0.000001, temperature=100;
/*   char puzzle9[9][9] = { {0, 0, 0, 0, 0, 0, 0, 0, 0},
      {0, 0, 3, 6, 0, 9, 7, 0, 0},
      {0, 6, 0, 0, 2, 0, 0, 8, 0},
      {0, 9, 0, 4, 0, 3, 0, 6, 0},
      {0, 0, 4, 0, 6, 0, 1, 0, 0},
      {0, 5, 0, 1, 0, 8, 0, 7, 0},
      {0, 7, 0, 0, 3, 0, 0, 5, 0},
      {0, 0, 1, 5, 0, 4, 2, 0, 0},
      {0, 0, 0, 0, 0, 0, 0, 0, 0}};
   
      char puzzle25[25][25] = {{0,0,0,9,19,0,0,0,15,0,12,25,0,18,0,0,0,0,0,16,0,11,14,0,17},
   {7,0,15,18,0,0,0,5,0,6,0,0,24,2,20,8,1,19,23,0,0,0,0,0,4},
   {0,0,6,0,21,0,1,9,2,14,0,23,0,0,0,7,0,17,24,0,0,19,0,0,12},
   {4,11,0,23,0,20,0,0,7,0,13,5,17,14,0,0,2,0,0,12,0,0,0,24,25},
   {0,2,0,0,0,19,24,0,0,16,1,0,0,7,4,11,0,0,22,0,0,21,0,0,0},
   {0,22,0,4,10,23,14,0,0,0,25,16,0,11,0,17,0,0,0,0,0,24,0,21,0},
   {19,25,21,6,0,0,0,0,5,0,2,0,0,9,0,0,0,0,12,0,13,0,0,0,16},
   {15,8,0,0,0,0,9,13,6,0,18,0,4,0,0,2,20,0,1,0,0,0,0,0,0},
   {0,7,9,14,0,15,16,21,0,0,22,0,0,0,0,0,0,13,0,10,0,25,6,0,0},
   {0,0,23,0,0,0,8,0,0,0,0,0,0,0,0,0,19,14,0,0,0,0,0,1,10},
   {0,24,0,17,8,0,18,1,0,3,0,0,22,0,0,15,0,0,2,0,0,0,0,0,21},
   {23,3,0,5,0,9,0,10,0,0,20,21,2,0,0,0,0,4,0,11,6,0,18,0,0},
   {0,0,0,11,1,8,23,0,22,0,3,0,16,6,0,0,10,0,0,18,0,12,7,0,2},
   {0,0,0,0,0,6,25,12,0,2,0,0,0,0,0,1,7,3,19,0,0,0,5,0,0},
   {20,18,0,0,0,0,0,17,24,0,7,0,0,5,12,0,0,0,0,0,14,4,0,8,0},
   {0,10,20,0,0,0,0,18,0,0,0,17,1,0,23,0,0,0,0,22,11,14,2,0,0},
   {17,0,13,0,0,25,0,0,0,0,9,18,0,0,21,12,11,1,0,7,0,3,4,0,20},
   {0,0,8,0,5,0,0,0,0,0,0,0,3,0,7,0,16,0,21,0,25,10,0,18,6},
   {11,0,0,0,24,22,12,0,23,0,6,0,14,0,8,0,9,2,0,0,0,17,0,16,15},
   {14,0,16,2,15,0,0,8,0,4,0,10,12,0,0,0,25,0,0,20,0,22,0,0,1},
   {18,0,0,0,0,0,0,22,0,23,14,0,20,0,0,0,0,0,0,2,7,9,0,0,0},
   {24,0,4,0,25,0,0,0,0,7,10,0,0,15,0,21,0,0,0,9,0,8,23,0,0},
   {10,0,12,0,22,0,3,0,0,0,23,0,19,8,0,20,14,16,0,5,0,6,21,0,0},
   {0,19,0,0,7,10,0,0,0,0,21,0,0,0,0,18,6,0,0,1,0,13,0,0,0},
   {0,21,0,0,0,5,11,0,0,0,0,3,0,0,18,4,17,0,0,0,22,0,0,0,0}};
*/
   if (argc>=3) {
      stoppingTemp=atof(argv[1]); rate=atof(argv[2]); temperature=atof(argv[3]);
   }
   srand(time(NULL));
   start = clock();
   if(SIZE==9){
      //if (anneal(puzzle9, stoppingTemp, rate, temperature)) { finish = clock();printPuzzle(best);
      //} else{ finish = clock(); printf("\n\nNO SOLUTION\n\n");}
   } else if(SIZE==16){
      if (anneal(stoppingTemp,rate, temperature)) { finish = clock();printPuzzle(best);
      } else{ finish = clock(); printf("\n\nNO SOLUTION\n\n");}
   } else{
      //if (anneal(puzzle25, stoppingTemp,rate, temperature)) { finish = clock();printPuzzle(best);
      //} else{ finish = clock(); printf("\n\nNO SOLUTION\n\n");}
   }
   printf("StartTime:%ld, EndTime:%ld, ExecutionTime:%f\n", start, finish, (double)(finish - start) / CLOCKS_PER_SEC);
}

/*
 
 | 2| 7| 4| 3|10|12|13| 8|11| 1| 9|15| 6| 5|14|16|
 | 1|12| 6|10| 9|11| 3| 5| 2|14| 7|16| 4|13| 8|15|
 | 5| 9| 8|16|15| 2| 7|14| 6| 4|13|12| 3|10| 1|11|
 |15|13|11|14|16| 1| 6| 4| 5|10| 3| 8| 9| 7| 2|12|
 +-----+-----+-----+
 |11|10| 1|12| 5|16|14| 7| 8| 6| 4| 3| 2|15| 9|13|
 |13| 2| 3| 7| 6|10|15| 1|12| 5|14| 9|11| 4|16| 8|
 |14| 6|16|15| 4| 8| 9| 3|13|11| 2| 7|10| 1|12| 5|
 | 9| 4| 5| 8|11|13|12| 2| 1|15|16|10| 7|14| 6| 3|
 +-----+-----+-----+
 |12|14|15|13| 2| 3| 5| 9|10|16|11| 6| 1| 8| 7| 4|
 | 7| 1| 9| 4| 8| 6|16|11|15| 3|12|13|14| 2| 5|10|
 |16| 3| 2| 6|12|14| 1|10| 4| 7| 8| 5|15|11|13| 9|
 | 8|11|10| 5| 7|15| 4|13|14| 9| 1| 2|16|12| 3| 6|
 +-----+-----+-----+
 |10| 5|12| 9|13| 4|11|16| 7| 2| 6| 1| 8| 3|15|14|
 | 4| 8|14| 1| 3| 5| 2| 6| 9|12|15|11|13|16|10| 7|
 | 6|16|13|11| 1| 7|10|15| 3| 8| 5|14|12| 9| 4| 2|
 | 3|15| 7| 2|14| 9| 8|12|16|13|10| 4| 5| 6|11| 1|
 
 {{0, 0, 0, 0, 0, 0, 0, 12, 0, 7, 0, 0, 0, 0, 0, 13},
 {3, 0, 0, 0, 0, 14, 15, 0, 0, 13, 0, 5, 16, 6, 10, 0},
 {11, 14, 0, 0, 7, 1, 9, 0, 0, 3, 15, 0, 0, 4, 12, 0},
 {0, 0, 0, 0, 0, 13, 2, 0, 4, 9, 0, 0, 0, 0, 1, 0},
 {0, 15, 1, 0, 0, 0, 0, 0, 10, 0, 11, 0, 0, 9, 0, 7},
 {16, 0, 3, 0, 0, 6, 5, 1, 9, 0, 7, 0, 0, 0, 0, 2},
 {13, 0, 0, 14, 0, 12, 0, 0, 0, 0, 0, 6, 0, 0, 5, 0},
 {8, 0, 0, 2, 0, 0, 3, 7, 0, 4, 0, 0, 0, 12, 14, 0},
 {9, 0, 0, 0, 0, 2, 0, 11, 6, 0, 1, 0, 0, 0, 0, 0},
 {15, 2, 12, 0, 0, 5, 0, 3, 0, 14, 0, 0, 9, 0, 6, 1},
 {0, 0, 0, 5, 9, 0, 0, 0, 2, 8, 0, 3, 0, 0, 0, 0},
 {0, 0, 14, 0, 0, 0, 0, 0, 0, 12, 0, 15, 3, 0, 0, 0},
 {0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 10, 3, 2, 0},
 {0, 0, 11, 0, 12, 3, 0, 0, 13, 0, 0, 0, 0, 0, 15, 5},
 {0, 6, 0, 3, 2, 4, 0, 0, 7, 0, 0, 0, 0, 16, 0, 0},
 {0, 1, 15, 0, 13, 0, 11, 16, 12, 10, 0, 0, 6, 8, 7, 0}};
 
 {0,0,0,9,19,0,0,0,15,0,12,25,0,18,0,0,0,0,0,16,0,11,14,0,17},
 {7,0,15,18,0,0,0,5,0,6,0,0,24,2,20,8,1,19,23,0,0,0,0,0,4},
 {0,0,6,0,21,0,1,9,2,14,0,23,0,0,0,7,0,17,24,0,0,19,0,0,12},
 {4,11,0,23,0,20,0,0,7,0,13,5,17,14,0,0,2,0,0,12,0,0,0,24,25},
 {0,2,0,0,0,19,24,0,0,16,1,0,0,7,4,11,0,0,22,0,0,21,0,0,0},
 {0,22,0,4,10,23,14,0,0,0,25,16,0,11,0,17,0,0,0,0,0,24,0,21,0},
 {19,25,21,6,0,0,0,0,5,0,2,0,0,9,0,0,0,0,12,0,13,0,0,0,16},
 {15,8,0,0,0,0,9,13,6,0,18,0,4,0,0,2,20,0,1,0,0,0,0,0,0},
 {0,7,9,14,0,15,16,21,0,0,22,0,0,0,0,0,0,13,0,10,0,25,6,0,0},
 {0,0,23,0,0,0,8,0,0,0,0,0,0,0,0,0,19,14,0,0,0,0,0,1,10},
 {0,24,0,17,8,0,18,1,0,3,0,0,22,0,0,15,0,0,2,0,0,0,0,0,21},
 {23,3,0,5,0,9,0,10,0,0,20,21,2,0,0,0,0,4,0,11,6,0,18,0,0},
 {0,0,0,11,1,8,23,0,22,0,3,0,16,6,0,0,10,0,0,18,0,12,7,0,2},
 {0,0,0,0,0,6,25,12,0,2,0,0,0,0,0,1,7,3,19,0,0,0,5,0,0},
 {20,18,0,0,0,0,0,17,24,0,7,0,0,5,12,0,0,0,0,0,14,4,0,8,0},
 {0,10,20,0,0,0,0,18,0,0,0,17,1,0,23,0,0,0,0,22,11,14,2,0,0},
 {17,0,13,0,0,25,0,0,0,0,9,18,0,0,21,12,11,1,0,7,0,3,4,0,20},
 {0,0,8,0,5,0,0,0,0,0,0,0,3,0,7,0,16,0,21,0,25,10,0,18,6},
 {11,0,0,0,24,22,12,0,23,0,6,0,14,0,8,0,9,2,0,0,0,17,0,16,15},
 {14,0,16,2,15,0,0,8,0,4,0,10,12,0,0,0,25,0,0,20,0,22,0,0,1},
 {18,0,0,0,0,0,0,22,0,23,14,0,20,0,0,0,0,0,0,2,7,9,0,0,0},
 {24,0,4,0,25,0,0,0,0,7,10,0,0,15,0,21,0,0,0,9,0,8,23,0,0},
 {10,0,12,0,22,0,3,0,0,0,23,0,19,8,0,20,14,16,0,5,0,6,21,0,0},
 {0,19,0,0,7,10,0,0,0,0,21,0,0,0,0,18,6,0,0,1,0,13,0,0,0},
 {0,21,0,0,0,5,11,0,0,0,0,3,0,0,18,4,17,0,0,0,22,0,0,0,0}};
 
 a	e	y	9	19	d	w	x	15	h	12	25	v	18	f	c	m	u	j	16	b	11	14	g	17
 7	r	15	18	o	v	m	5	l	6	k	w	24	2	20	8	1	19	23	n	j	p	c	i	4
 h	u	6	n	21	k	1	9	2	14	q	23	j	c	p	7	s	17	24	d	e	19	w	m	12
 4	11	w	23	p	20	j	c	7	s	13	5	17	14	t	i	2	v	f	12	a	q	h	24	25
 l	2	c	j	m	19	24	n	r	16	1	h	i	7	4	11	e	q	22	o	x	21	u	f	s
 e	22	a	4	10	23	14	b	t	u	25	16	g	11	m	17	c	f	s	h	q	24	l	21	i
 19	25	21	6	u	c	d	k	5	j	2	q	h	9	a	w	x	g	12	y	13	s	r	o	16
 15	8	k	p	r	y	9	13	6	l	18	o	4	j	e	2	20	n	1	v	c	g	t	w	x
 c	7	9	14	s	15	16	21	a	r	22	t	x	l	y	e	d	13	k	10	u	25	6	b	h
 m	l	23	y	b	s	8	g	n	w	r	u	f	v	c	p	19	14	i	q	d	e	k	1	10
 f	24	g	17	8	o	18	1	u	3	d	k	22	t	j	15	l	e	2	m	i	x	p	n	21
 23	3	n	5	l	9	g	10	m	q	20	21	2	r	o	y	h	4	p	11	6	a	18	t	w
 v	m	t	11	1	8	23	d	22	e	3	y	16	6	n	o	10	i	u	18	r	12	7	q	2
 p	o	j	w	d	6	25	12	v	2	h	i	s	x	q	1	7	3	19	r	y	u	5	k	m
 20	18	b	q	i	p	t	17	24	k	7	a	m	5	12	x	v	w	n	f	14	4	j	8	c
 n	10	20	v	c	m	e	18	i	t	y	17	1	p	23	f	q	h	d	22	11	14	2	l	g
 17	x	13	t	f	25	b	p	j	y	9	18	q	w	21	12	11	1	o	7	h	3	4	e	20
 w	d	8	l	5	r	q	o	k	a	t	b	3	u	7	m	16	y	21	x	25	10	i	18	6
 11	a	s	g	24	22	12	u	23	v	6	d	14	n	8	j	9	2	e	c	t	17	m	16	15
 14	i	16	2	15	g	f	8	c	4	e	10	12	m	k	t	25	s	r	20	v	22	y	x	1
 18	f	e	h	k	l	v	22	p	23	14	m	20	d	r	n	y	j	q	2	7	9	a	c	t
 24	p	4	m	25	a	r	t	s	7	10	f	e	15	b	21	w	k	c	9	l	8	23	u	o
 10	q	12	a	22	b	3	y	d	n	23	g	19	8	i	20	14	16	m	5	s	6	21	r	k
 b	19	r	c	7	10	u	q	o	i	21	l	k	y	w	18	6	x	h	1	p	13	n	d	e
 i	21	o	u	x	5	11	f	h	m	p	3	n	a	18	4	17	l	g	t	22	b	q	j	y
 */