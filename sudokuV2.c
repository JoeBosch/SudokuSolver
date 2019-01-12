#include "sudokuV2.h"

const int bitField[10] = {0, 1, 2, 4, 8, 16, 32, 64, 128, 256};

int main(int argc, char** argv)
{   cell sudokuBoard[9][9];
    //Gets intial board and prepares for solving.
    if(argc != 2)
    {   printf("Use the file format: %s <File Name>\n", argv[0]);
        return -1;
    }
    readFile(sudokuBoard, argv[1]);
    int placedCount = 0;
    for(int i = 0; i < 9; i++)
        for(int j = 0; j < 9; j++)
            if(sudokuBoard[i][j].value > 0)
            {   sudokuBoard[i][j].values = 0;
                applyMask(sudokuBoard, i, j);
                placedCount++;
            }
    placedCount = solveBoard(sudokuBoard, placedCount);
    if(placedCount == 81)
        printBoard(sudokuBoard);
    else
        printf("No solution\n");
    return 0;
}

//Remove the bit from all numbers affected by the value at (x,y)
void applyMask(cell sudokuBoard[9][9], int x, int y)
{   int mask = ~bitField[sudokuBoard[x][y].value]; // The complement of the bit related to the value in the cell.

    for(int maskP = 0; maskP < 9; maskP++)
    {   sudokuBoard[maskP][y].values &= mask; // bitwise and each cell in the same row and column to remove the bit of the mask.
        sudokuBoard[x][maskP].values &= mask;
    }

    int sectionX1;
    int sectionX2;
    getSection(x, &sectionX1, &sectionX2);
    int sectionY1;
    int sectionY2;
    getSection(y, &sectionY1, &sectionY2);

    sudokuBoard[sectionX1][sectionY1].values &= mask; // bit wise and each cell in the same section to remove the bit of the mask.
    sudokuBoard[sectionX1][sectionY2].values &= mask;
    sudokuBoard[sectionX2][sectionY1].values &= mask;
    sudokuBoard[sectionX2][sectionY2].values &= mask;
}

//If only one possible value for the cell exists then enter the value in the cell
int onlyMove(cell sudokuBoard[9][9])
{   int moves = 0;
    for(int x = 0; x < 9; x++)
        for(int y = 0; y < 9; y++)
        {   int allowedValues = sudokuBoard[x][y].values;
            if(countBits(allowedValues) == 1)
            {   setValue(sudokuBoard, getBitIndex(allowedValues), x, y);
                moves++;
            }
        }
    return moves;
}

//If there is only one position in a section that can contain a number then enter the value in the cell
int onlySection(cell sudokuBoard[9][9])
{   int moves = 0;
    for(int sectionX = 0; sectionX < 9; sectionX += 3)
        for(int sectionY = 0; sectionY < 9; sectionY += 3)
            for(int value = 1; value <= 9; value++)
            {   int allowedX = -1;
                int allowedY = -1;
                int endX = sectionX + 3;
                int endY = sectionY + 3;
                int allowedBit = bitField[value];

                for(int x = sectionX; x < endX; x++)
                {   for(int y = sectionY; y < endY; y++)
                    {   if((sudokuBoard[x][y].values & allowedBit) > 0)
                        {   if(allowedX < 0)
                            {   allowedX = x;
                                allowedY = y;
                            }
                            else
                            {   allowedX = -1;
                                goto section; //Double for-loop break
                            }
                        }
                    }
                }
                if(allowedX >= 0)
                {   setValue(sudokuBoard, value, allowedX, allowedY);
                    moves++;
                }
                section:;
            }
    return moves;
}

//If only one position in a row or column can contain a certain value then enter the value in the cell
int onlyRowColumn(cell sudokuBoard[9][9])
{   int moves = 0;
    for(int value = 1; value <= 9; value++)
    {   for(int x = 0; x < 9; x++)
        {   int allowedY = -1;
            for(int y = 0; y < 9; y++)
            {   if((sudokuBoard[x][y].values & bitField[value]) > 0)
                {   if(allowedY < 0)
                        allowedY = y;
                    else
                    {   allowedY = -1;
                        break;
                    }
                }
            }
            if(allowedY >= 0)
            {   setValue(sudokuBoard, value, x, allowedY);
                moves++;
            }
        }
        for(int y = 0; y < 9; y++)
        {   int allowedX = -1;
            for(int x = 0; x < 9; x++)
            {   if((sudokuBoard[x][y].values & bitField[value]) > 0)
                {   if(allowedX < 0)
                        allowedX = x;
                    else
                    {   allowedX = -1;
                        break;
                    }
                }
            }
            if(allowedX >= 0)
            {   setValue(sudokuBoard, value, allowedX, y);
                moves++;
            }
        }
    }
    return moves;
}

//Used when the rules no longer can provide a definitive answer. Tries a value that doesn't break the
//rules of sudoku and sees if a solution exists with that value otherwise backtracks.
int bruteForce(cell bruteBoard[9][9], int placedCount)
{   for(int x = 0; x < 9; x++)
        for(int y = 0; y < 9; y++)
            if(bruteBoard[x][y].value == 0)
            {   for(int value = 1; value <= 9; value++)
                    if((bruteBoard[x][y].values & bitField[value]) > 0)
                    {   cell testBoard[9][9];
                        for(int i = 0; i < 9; i++)
                            for(int j = 0; j < 9; j++)
                            {   testBoard[i][j].value = bruteBoard[i][j].value;
                                testBoard[i][j].values = bruteBoard[i][j].values;
                            }
                        setValue(testBoard, value, x, y);
                        int placed = solveBoard(testBoard, placedCount + 1);
                        if(placed == 81)
                        {   for(int i = 0; i < 9; i++)
                                for(int j = 0; j < 9; j++)
                                    bruteBoard[i][j].value = testBoard[i][j].value;
                            return placed;
                        }
                    }
                return placedCount;
            }
}

//Tries the rules to find any given positions otherwise creates a new board state to try and bruteforce.
int solveBoard(cell sudokuBoard[9][9], int placedCount)
{   int lastPlacedCount = 0;
    while(placedCount - lastPlacedCount > 3 && placedCount < 70 && placedCount > 10)
    {   lastPlacedCount = placedCount;
        placedCount += onlyMove(sudokuBoard);
        placedCount += onlySection(sudokuBoard);
        placedCount += onlyRowColumn(sudokuBoard);
    }

    if(placedCount < 81)
    {   cell bruteBoard[9][9];
        for(int x = 0; x < 9; x++)
            for(int y = 0; y < 9; y++)
            {   bruteBoard[x][y].value = sudokuBoard[x][y].value;
                bruteBoard[x][y].values = sudokuBoard[x][y].values;
            }
        if(bruteForce(bruteBoard, placedCount) == 81)
        {   placedCount = 81;
            for(int x = 0; x < 9; x++)
                for(int y = 0; y < 9; y++)
                    sudokuBoard[x][y].value = bruteBoard[x][y].value;
        }
    }
    return placedCount;
}

//Given a position in a section finds the position of other cells not in the same row or column.
void getSection(int pos, int *section1, int *section2)
{   switch(pos % 3){
        case 0:
            *section1 = pos + 1;
            *section2 = pos + 2;
            break;
        case 1:
            *section1 = pos - 1;
            *section2 = pos + 1;
            break;
        case 2:
            *section1 = pos - 2;
            *section2 = pos - 1;
            break;
    }
}

//Removes last bit from number until all bits are removed.
//eg. 5 = 0101 4 = 0100
//    5&4 = 0100 3= 0011
//    4&3 = 0000
int countBits(int value)
{   int count = 0;
    while(value > 0)
    {   value = value & (value - 1);
        count++;
    }
    return count;
}

//Set the value of the cell and remove the value from possible values of other cells.
void setValue(cell sudokuBoard[9][9], int value, int x, int y)
{   sudokuBoard[x][y].value = value;
    sudokuBoard[x][y].values = 0;
    applyMask(sudokuBoard, x, y);
}

//Counts the number of bitshifts to zero the values
int getBitIndex(int value)
{   int index = 0;
    while(value > 0)
    {   index++;
        value >>= 1;
    }
    return index;
}

void printBoard(cell sudokuBoard[9][9])
{   for(int x = 0; x < 9; x++)
    {   for(int y = 0; y < 9; y++)
            printf("%d ", sudokuBoard[x][y].value);
        printf("\n");
    }
}

void readFile(cell sudokuBoard[9][9], char* fileName)
{   FILE *f = fopen(fileName, "r");
    if(f == NULL)
    {   printf("The file \"%s\" does not exist.\n", fileName);
        exit(-1);
    }
    for(char row = 0; row < 9; row++)
        for(char col = 0; col < 9; col++)
        {   fscanf(f, "%d", &sudokuBoard[row][col].value);
            sudokuBoard[row][col].values = 511; //0001 1111 1111
        }
}