#ifndef SUDOKU
#define SUDOKU

#include <stdio.h>
#include <stdlib.h>

typedef struct Cell
{   int values; // Last 9 bits correspond to possible numbers for cell
    int value;
} cell;

void readFile(cell sudokuBoard[9][9], char* fileName);
void getSection(int pos, int *section1, int *section2);
void applyMask(cell sudokuBoard[9][9], int x, int y);
int countBits(int value);
void setValue(cell sudokuBoard[9][9], int value, int x, int y);
int getBitIndex(int value);
int onlyMove(cell sudokuBoard[9][9]);
int onlySection(cell sudokuBoard[9][9]);
int onlyRowColumn(cell sudokuBoard[9][9]);
int bruteForce(cell bruteBoard[9][9], int placedCount);
int solveBoard(cell sudokuBoard[9][9], int placedCount);
void printBoard(cell sudokuBoard[9][9]);

#endif