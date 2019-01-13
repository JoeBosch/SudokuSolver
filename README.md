# SudokuSolver

Sudoku Solver V1 and V2 are both backtracking algorithims to solve Sudoku puzzles. Version 1 uses only bruteforce backtracking whereas Version 2 is also identifies logical necessities.

### Challenges faced in version 2:
  - Whereas version 1 only allows one cell to be updated at a time version 2 allows multiple which makes backtracking harder as reversing the rules cannot be done without previous knowledge.
  - Each cell on the board requires information of the other cells affecting itself.
  
### Solutions to the challenges in version 2:
  - Backtracking is possible by creating a new board state when attempting to bruteforce and if unsuccessful reverting to a previous board state.
  - Each cell contains the information of the surrounding cells with a variable called values with each bit representing a possible value for the cell. When a cell is updated it changes the values variable of all cells it influences.
