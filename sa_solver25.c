#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <math.h>

#define SIZE 25
#define N 5
#define MAX_SWAP 1

void transform(int swapCount);
int delta();
int costF();
char isVld(int row, int col, char num);
void accept();
void copy(char src[][SIZE], char dest[][SIZE]);
void initialize();
void checkForElimination(char numbers[]);
void checkForLoneRanger(char numbers[]);
void elimination(char numbers[], int row, int col, char num);
void printPuzzle();
void printAux();

struct coor{ char x, y;};

char aux[SIZE][SIZE][SIZE + 3];
struct coor map[MAX_SWAP][2];
//sortedIndexes[SIZE+1][SIZE*SIZE/2]
char best[SIZE][SIZE], neighbor[SIZE][SIZE], puzzle[SIZE][SIZE] =  {{0, 0, 0, 9, 20, 0, 0, 0, 17, 0, 12, 14, 0, 19, 0, 0, 0, 0, 0, 16, 0, 11, 15, 0, 18},
   {7, 0, 17, 19, 0, 0, 0, 5, 0, 6, 0, 0, 25, 2, 21, 8, 1, 20, 24, 0, 0, 0, 0, 0, 4},
   {0, 0, 6, 0, 22, 0, 1, 9, 2, 15, 0, 24, 0, 0, 0, 7, 0, 18, 25, 0, 0, 20, 0, 0, 12},
   {4, 11, 0, 24, 0, 21, 0, 0, 7, 0, 13, 5, 18, 15, 0, 0, 2, 0, 0, 12, 0, 0, 0, 25, 14},
   {0, 2, 0, 0, 0, 20, 25, 0, 0, 16, 1, 0, 0, 7, 4, 11, 0, 0, 23, 0, 0, 22, 0, 0, 0},
   {0, 23, 0, 4, 10, 24, 15, 0, 0, 0, 14, 16, 0, 11, 0, 18, 0, 0, 0, 0, 0, 25, 0, 22, 0},
   {20, 14, 22, 6, 0, 0, 0, 0, 5, 0, 2, 0, 0, 9, 0, 0, 0, 0, 12, 0, 13, 0, 0, 0, 16},
   {17, 8, 0, 0, 0, 0, 9, 13, 6, 0, 19, 0, 4, 0, 0, 2, 21, 0, 1, 0, 0, 0, 0, 0, 0},
   {0, 7, 9, 15, 0, 17, 16, 22, 0, 0, 23, 0, 0, 0, 0, 0, 0, 13, 0, 10, 0, 14, 6, 0, 0},
   {0, 0, 24, 0, 0, 0, 8, 0, 0, 0, 0, 0, 0, 0, 0, 0, 20, 15, 0, 0, 0, 0, 0, 1, 10},
   {0, 25, 0, 18, 8, 0, 19, 1, 0, 3, 0, 0, 23, 0, 0, 17, 0, 0, 2, 0, 0, 0, 0, 0, 22},
   {24, 3, 0, 5, 0, 9, 0, 10, 0, 0, 21, 22, 2, 0, 0, 0, 0, 4, 0, 11, 6, 0, 19, 0, 0},
   {0, 0, 0, 11, 1, 8, 24, 0, 23, 0, 3, 0, 16, 6, 0, 0, 10, 0, 0, 19, 0, 12, 7, 0, 2},
   {0, 0, 0, 0, 0, 6, 14, 12, 0, 2, 0, 0, 0, 0, 0, 1, 7, 3, 20, 0, 0, 0, 5, 0, 0},
   {21, 19, 0, 0, 0, 0, 0, 18, 25, 0, 7, 0, 0, 5, 12, 0, 0, 0, 0, 0, 15, 4, 0, 8, 0},
   {0, 10, 21, 0, 0, 0, 0, 19, 0, 0, 0, 18, 1, 0, 24, 0, 0, 0, 0, 23, 11, 15, 2, 0, 0},
   {18, 0, 13, 0, 0, 14, 0, 0, 0, 0, 9, 19, 0, 0, 22, 12, 11, 1, 0, 7, 0, 3, 4, 0, 21},
   {0, 0, 8, 0, 5, 0, 0, 0, 0, 0, 0, 0, 3, 0, 7, 0, 16, 0, 22, 0, 14, 10, 0, 19, 6},
   {11, 0, 0, 0, 25, 23, 12, 0, 24, 0, 6, 0, 15, 0, 8, 0, 9, 2, 0, 0, 0, 18, 0, 16, 17},
   {15, 0, 16, 2, 17, 0, 0, 8, 0, 4, 0, 10, 12, 0, 0, 0, 14, 0, 0, 21, 0, 23, 0, 0, 1},
   {19, 0, 0, 0, 0, 0, 0, 23, 0, 24, 15, 0, 21, 0, 0, 0, 0, 0, 0, 2, 7, 9, 0, 0, 0},
   {25, 0, 4, 0, 14, 0, 0, 0, 0, 7, 10, 0, 0, 17, 0, 22, 0, 0, 0, 9, 0, 8, 24, 0, 0},
   {10, 0, 12, 0, 23, 0, 3, 0, 0, 0, 24, 0, 20, 8, 0, 21, 15, 16, 0, 5, 0, 6, 22, 0, 0},
   {0, 20, 0, 0, 7, 10, 0, 0, 0, 0, 22, 0, 0, 0, 0, 19, 6, 0, 0, 1, 0, 13, 0, 0, 0},
   {0, 22, 0, 0, 0, 5, 11, 0, 0, 0, 0, 3, 0, 0, 19, 4, 18, 0, 0, 0, 23, 0, 0, 0, 0}};

int anneal(double stoppingTemp, double rate, double temperature) {
   int dlta, costB, cost;
   initialize();
   copy(puzzle, best);
   cost = costF();
   costB = cost;
   printf("Cost:%d\n",costB);
   while (cost > 0 && stoppingTemp < temperature) {
      transform(MAX_SWAP);
      dlta = delta();
      if (dlta < 0 || (((double)(rand() % 1000) / 1000.0) <= exp(-dlta / temperature))) {
         accept();
         cost +=dlta;
         if (costB > cost){
            costB = cost;
            copy(puzzle, best);
         }
      }
      temperature -= rate * temperature;
   }
   copy(best, puzzle);
   printf("CostB:%d\n", costB);
   return costB ? 0 : 1;
}

void transform(int swapCount) {
   int i, j, row1, row2, col1, col2,randomNumber;
   copy(puzzle, neighbor);
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
   int x, y, coorX, coorY;
   char z, numbers[SIZE+1]={0}, num = 0, counters[SIZE]={0},found;
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
   //printAux();
   //printf("\n");
   checkForElimination(numbers);
   //printAux();
   //printf("\n");
   checkForLoneRanger(numbers);
   //printAux();
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
   int x,y,z, counters[SIZE]={0};
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
         }
      }
   }
}

void checkForLoneRanger(char numbers[]) {
   int i,j,num, row, col, colStart, rowStart, hAlone=0, hIndex, hFound, vAlone=0, sAlone=0, vIndex, sIndexX, sIndexY, vFound, sFound;
   for (num=1; num<=SIZE; num++) {
      for (i = 0; i < SIZE; i++) {
         hFound=0; vFound=0; sFound=0; hAlone=0; vAlone=0; sAlone=0;
         rowStart = (i / N) * N; colStart = (i / N) * N;
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
            if (puzzle[rowStart + (j % N)][colStart + (j / N)]==num) {
               sFound=1;
            } else if (aux[rowStart + (j % N)][colStart + (j / N)][num] && !sFound) {
               sAlone++;
               sIndexX=rowStart + (j % N);
               sIndexY=colStart + (j / N);
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
               if (aux[j][hIndex][0]==1)
                  elimination(numbers, j, hIndex, num);
            }
            aux[i][hIndex][SIZE + 1]=SIZE + 1;
            aux[i][hIndex][0]=0;
            numbers[num]++;
            checkForLoneRanger(numbers);
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
               if (aux[vIndex][j][0]==1)
                  elimination(numbers,vIndex, j,num);
            }
            aux[vIndex][i][SIZE + 1]=SIZE + 1;
            aux[vIndex][i][0]=0;
            numbers[num]++;
            checkForLoneRanger(numbers);
         }
         if (!sFound && sAlone==1) {
            puzzle[sIndexX][sIndexY]=num;
            for (j=0; j<=SIZE; j++) {
               aux[sIndexX][sIndexY][j]=0;
               
               aux[j][sIndexY][0] = aux[j][sIndexY][0] - aux[j][sIndexY][num];
               aux[j][sIndexY][num]=0;
               
               aux[sIndexX][j][0] = aux[sIndexX][j][0] - aux[sIndexX][j][num];
               aux[j][sIndexY][num]=0;
               
               if (aux[sIndexX][j][0]==1)
                  elimination(numbers, sIndexX, j, num);
               if (aux[j][sIndexY][0]==1)
                  elimination(numbers, j, sIndexY, num);
            }
            aux[sIndexX][sIndexY][SIZE + 1]=SIZE + 1;
            aux[sIndexX][sIndexY][0]=0;
            numbers[num]++;
            checkForLoneRanger(numbers);
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

void copy(char src[][SIZE], char dest[][SIZE]) {
   int i,j;
   for (i = 0; i < SIZE; i++)
      for (j = 0; j < SIZE; j++)
         dest[i][j] = src[i][j];
}

char isVld(int row, int col, char num) {
   int i, rowStart = (row / N) * N, colStart = (col / N) * N;
   for (i = 0; i < SIZE; ++i)
      if ((puzzle[row][i] == num) || (puzzle[i][col] == num) ||
          (puzzle[rowStart + (i % N)][colStart + (i / N)] == num)) return 0;
   return 1;
}

void printPuzzle(){
   int i,j;
   printf("\n+-----+-----+-----+\n");
   for(i = 1; i <= SIZE; ++i) {
      printf("%2d. ",i-1);
      for(j = 1; j <= SIZE; ++j) printf("|%2d", puzzle[i-1][j-1]);
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
   int found=0;
   clock_t finish, start;
   double stoppingTemp=0.000000001, rate=0.0000001, temperature=100;

   srand(time(NULL));
   if (argc>=3) {
      stoppingTemp=atof(argv[1]); rate=atof(argv[2]); temperature=atof(argv[3]);
   }
   start = clock();
   if (anneal(stoppingTemp, rate, temperature)) { finish = clock();printPuzzle();
   } else{ finish = clock(); printf("\n\nNO SOLUTION\n\n");}
   printf("StartTime:%ld, EndTime:%ld, ExecutionTime:%f\n", start, finish, (double)(finish - start) / CLOCKS_PER_SEC);
}