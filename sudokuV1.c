#include <stdio.h>
#include <stdbool.h>
#include <sys/time.h>

#define BOARD 81

//Check if a number would conflict with any other number on the board.
bool isValid(char num, char sudokuBoard[BOARD], char cell)
{   char rowStart = (cell / 9) * 9;
    char columnStart = cell % 9;
    char boxRowStart = (cell / 27) * 27;
    char boxColStart = (columnStart / 3) * 3;
    for(char i = 0; i < 9; i++)
    {   //Check if any row conflicts
        if(sudokuBoard[rowStart + i] == num) return false;
        //Check if any column conflicts
        if(sudokuBoard[columnStart + i * 9] == num) return false;
        //Check if any box conflicts
        //Using a 9x9 array instead would make this easier to read but solve harder to read
        char rowOffset =  (i / 3) * 9;
        char colOffset = i % 3;
        if(sudokuBoard[boxRowStart + boxColStart + rowOffset + colOffset] == num) return false;
    }
    return true;
}

//Puts numbers until it cant then goes back until it can
bool solve(char sudokuBoard[BOARD], char cell)
{   //Find empty cell
    while(sudokuBoard[cell] != 0 && cell < BOARD)
        cell++;
    cell == BOARD ? return true:;
    for(char num = 1; num < 10; num++)
    {   if(isValid(num, sudokuBoard, cell))
        {   //Assume the number is in the correct position and move onto the next cell.
            sudokuBoard[cell] = num;
            if(solve(sudokuBoard, cell + 1))
                return true;
            //The number did not provide a solution with the previous assumptions so remove the number and try again.
            sudokuBoard[cell] = 0;
        }
    }
    //No number works with the previous assumptions therefore atleast one of the previous assumptions is false or there is no solution.
    return false;
}

void printBoard(char sudokuBoard[BOARD])
{   printf("\n");
    for(char i = 0; i < BOARD; i++)
    {   printf("%d ", sudokuBoard[i]);
        (i + 1) % 9 ?:printf("\n");
    }
}

//Easiest way to input is sudoku_jbosch < sudoku<number>.txt
void readFile(char sudokuBoard[BOARD])
{   int num;
    printf("Enter board(designed for sudokuV1 < sudoku<number>.txt): ");
    char i = 0;
    while(scanf("%d", &num) && i < BOARD)
        sudokuBoard[i++] = num;
}


//Char is used as much as possible as int requires 4 bytes and 3 are wasted
int main()
{   char sudokuBoard[BOARD];
    readFile(sudokuBoard);
    if(solve(sudokuBoard, 0))
        printBoard(sudokuBoard);
    else
        printf("Something went wrong\n");
    return 0;
}