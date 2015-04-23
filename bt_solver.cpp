#include <stdio.h>

#define UNASSIGNED 0
#define N 16
#define SUB 4

bool FindUnassignedLocation(int grid[N][N], int &row, int &col);
bool isSafe(int grid[N][N], int row, int col, int num);

bool SolveSudoku(int grid[N][N]) {
   int row, col, num;
   
   if (!FindUnassignedLocation(grid, row, col))
   return true;
   
   for (num = 1; num <= N; num++) {
      if (isSafe(grid, row, col, num)) {
         grid[row][col] = num;
         if (SolveSudoku(grid))
            return true;
         grid[row][col] = UNASSIGNED;
      }
   }
   return false;
}

bool FindUnassignedLocation(int grid[N][N], int &row, int &col) {
   for (row = 0; row < N; row++)
   for (col = 0; col < N; col++)
   if (grid[row][col] == UNASSIGNED)
   return true;
   return false;
}

bool UsedInRow(int grid[N][N], int row, int num) {
   for (int col = 0; col < N; col++)
   if (grid[row][col] == num)
   return true;
   return false;
}

bool UsedInCol(int grid[N][N], int col, int num) {
   for (int row = 0; row < N; row++)
   if (grid[row][col] == num)
   return true;
   return false;
}

bool UsedInBox(int grid[N][N], int boxStartRow, int boxStartCol, int num) {
   for (int row = 0; row < SUB; row++)
   for (int col = 0; col < SUB; col++)
   if (grid[row+boxStartRow][col+boxStartCol] == num)
   return true;
   return false;
}

bool isSafe(int grid[N][N], int row, int col, int num) {
   return !UsedInRow(grid, row, num) && !UsedInCol(grid, col, num) &&
   !UsedInBox(grid, row - row%SUB , col - col%SUB, num);
}

void printGrid(int grid[N][N]) {
   for (int row = 0; row < N; row++) {
      for (int col = 0; col < N; col++)
      printf("%2d", grid[row][col]);
      printf("\n");
   }
}

int main() {
   int grid[N][N] = {{0, 0, 0, 8, 5, 0, 0, 0, 10, 3, 7, 1, 0, 0, 15, 2},
      {0, 9, 0, 7, 15, 0, 0, 11, 0, 0, 2, 0, 1, 0, 4, 0},
      {0, 0, 0, 4, 8, 0, 16, 0, 0, 0, 0, 0, 14, 11, 0, 13},
      {0, 0, 0, 16, 2, 0, 14, 10, 0, 13, 0, 5, 0, 0, 0, 0},
      {0, 1, 13, 0, 0, 0, 11, 0, 0, 9, 8, 10, 16, 5, 14, 12},
      {8, 10, 0, 0, 0, 0, 0, 0, 0, 0, 3, 7, 0, 15, 0, 0},
      {0, 0, 11, 0, 0, 3, 0, 0, 0, 15, 0, 0, 0, 0, 10, 9},
      {0, 14, 12, 0, 0, 8, 0, 15, 11, 0, 0, 0, 0, 0, 7, 0},
      {0, 6, 0, 0, 0, 0, 0, 0, 15, 7, 0, 0, 0, 0, 0, 0},
      {0, 0, 15, 10, 9, 14, 0, 0, 0, 0, 0, 0, 12, 6, 3, 0},
      {0, 0, 0, 12, 0, 0, 4, 13, 0, 0, 9, 0, 0, 0, 0, 0},
      {0, 11, 0, 0, 0, 2, 0, 8, 0, 0, 13, 0, 0, 7, 0, 0},
      {7, 0, 0, 0, 0, 0, 0, 5, 14, 0, 12, 9, 8, 0, 0, 0},
      {0, 12, 0, 0, 0, 0, 0, 0, 0, 4, 0, 2, 13, 10, 1, 0},
      {0, 15, 10, 0, 0, 0, 0, 0, 0, 1, 0, 3, 0, 0, 16, 6},
      {6, 0, 0, 14, 1, 0, 10, 0, 0, 0, 15, 13, 5, 0, 12, 4}};
   if (SolveSudoku(grid))
   printGrid(grid);
   else
   printf("No solution exists");
   
   return 0;
}