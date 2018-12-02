// SudokuGrid.h
#ifndef SODUKOGRID_H 
#define SODUKOGRID_H 
#include "Cell.h"
class SudokuGrid{
  public:
    SudokuGrid(); // Constructor
    ~SudokuGrid(); // Destructor
    void display(); // Display the board to the terminal window
    void solve(); // Solve the sudoku puzzle
    bool checkSolved(); // Check to see fit the puzzle is solved
  private:
    Cell Grid[9][9]; // The board
};
#endif
