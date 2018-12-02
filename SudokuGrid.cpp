#include "Cell.h" 
#include "SudokuGrid.h" 
#include <iostream> 
#include <string> 
#include <fstream> 
#include <array>
using namespace std; 
// Default Constructor 
// 1) Set Variables Needed for the Constructor 
// 2) Prompts the user for a file containing an unsolved puzzle and reads it in 
// 3) Fills the board and fills in Cell information for each slot in the grid 
// 4) Sets the possible options for each Cell 
SudokuGrid::SudokuGrid() {
  // Set Variables
  string infilename; // the name of the file the user wishes to include
  string theline; // each line in the incoming file
  ifstream ifs; // Allows the file to be accesses
  bool badfile = true; // If it is not set to false (in step 2), the file is unreadable or doesn't exist
  int c; // used to count columns in parts 3 and 4
  int r = 0; // used to count rows in parts 3 and 4
  int mg; // used to count minigrids in parts 3 and 4
  int v; // used to keep track of values in part 4
  int convertchar; // Used to keep the char number form of each number in the original file
  // Fill Grid with input
  cout << "Welcome to the Sudoku Solver!\n";
  while (badfile) {
    cout << "Enter the name of the Sudoku file: ";
    cin >> infilename; // asks for file name that contains unsolved puzzle
    ifs.open(infilename);
    if(!ifs) {
      cout << "error opening file " << infilename << endl << endl;
      badfile = true;
    }
    else
      badfile = false; // the file is able to be opened so we can continue
  }
  // Fills the board
  while(!ifs.eof()) {
    getline(ifs, theline); // each line of the original file is stored as 'theline' as we parse through the file
    c = 0;
    for (auto it = theline.begin(); it != theline.end(); it++) { // parse through 'theline'
      if (*it == ' ') { continue; } // skip spaces
      else {
        convertchar = *it - 48;; // convert from char number to normal intergers (i.e. 48 becomes 0)
        Grid[r][c].setValue(convertchar); // set the value of the cell
        Grid[r][c].setColumn(c); // tell the cell which column of the board it is a part of
        Grid[r][c].setRow(r); // tell the cell which row of the board it is a part of
        if ( (r < 3) && (c < 3) ) // if statement to determine whic minigrid the cell is a part of
          Grid[r][c].setMiniGrid(1); // minigrid 1
        else if ( (r < 3) && (c >= 3) && (c < 6) )
          Grid[r][c].setMiniGrid(2); // minigrid 2
        else if ( (r < 3) && (c >= 6) )
          Grid[r][c].setMiniGrid(3); // minigrid 3
        else if ( (r >= 3) && (r < 6) && (c < 3) )
          Grid[r][c].setMiniGrid(4); // minigrid 4
        else if ( (r >= 3) && (r < 6) && (c >= 3) && (c < 6) )
          Grid[r][c].setMiniGrid(5); // minigrid 5
        else if ( (r >= 3) && (r < 6) && (c >= 6) )
          Grid[r][c].setMiniGrid(6); // minigrid 6
        else if ( (r >= 6) && (c < 3) )
          Grid[r][c].setMiniGrid(7); // minigrid 7
        else if ( (r >= 6) && (c >= 3) && (c < 6) )
          Grid[r][c].setMiniGrid(8); // minigrid 8
        else if ( (r >= 6) && (c >= 6) )
          Grid[r][c].setMiniGrid(9); // minigrid 9
        else { Grid[r][c].setMiniGrid(0); }
        c += 1; // increase the column at each iteration of the inner for loop
      }
    }
    r += 1; // increase the row at each iteration of the outer while loop
  }
  // Removes already used values (in the same row/column/minigrid) from the options array in the Cell class
  for (int ROW = 0; ROW < 9; ROW++) {
    for (int COL = 0; COL < 9; COL++) {
      if (Grid[ROW][COL].getValue() == 0) { continue; } // parse through the whole grid... if a cell = 0, skip it
      else { // if the value of the cell != 0
        v = Grid[ROW][COL].getValue(); // save the value of the cell
        r = Grid[ROW][COL].getRow(); // save the row of the cell
        c = Grid[ROW][COL].getColumn(); // save the column of the cell
        mg = Grid[ROW][COL].getMiniGrid(); // save the minigrid of the cell
        for (int row = 0; row < 9; row++) {
          for (int col = 0; col < 9; col++) { // parse the entire grid again
            if (r == Grid[row][col].getRow()) { Grid[row][col].removeOption(v); } // if they are in the same row, remove the value from the options array
            else if (c == Grid[row][col].getColumn()) { Grid[row][col].removeOption(v); } // if they are in the same column, remove the value from options
            else if (mg == Grid[row][col].getMiniGrid()) { Grid[row][col].removeOption(v); } // if they are in the same minigrid, remove the value from options
          }
        }
      }
    }
  }
}
// Destructor 
SudokuGrid::~SudokuGrid() {
  // no variable is dynamically allocated, so no need to delete anything
}
// Displays board 
void SudokuGrid::display() {
  ofstream myfile;
  myfile.open("completed_sudoku.txt");
  for (int r = 0; r < 9; r++) {
    for (int c = 0; c < 9; c++) {
      myfile<< Grid[r][c].getValue();
    }
      myfile << "\n";
  }
}
// The computer will sove the Sudoku Puzzle 
void SudokuGrid::solve() {
  // Set variables
  bool one_value; // used to see if the cell has only one possible value in the options array
  int cnt; // used to keep track of the cells that cannot be solved normally (if cnt = 81, we have to use singletons)
  int option_cnt; // how many times a certain value (1-9) is a possible option when searching for singletons in a row/column
  int singleton; // the value of the singleton
  int gridtracker; // Used to save the row or column when solving for singletons (depends on what direction we are searching in)
  // This while loop is not finished until each cell has a non-zero value (aka solved)
  while (!checkSolved()) {
    cnt = 0; // set cnt to zero at the beginning of each iteration
    for (int row = 0; row < 9; row++) {
      for (int col = 0; col < 9; col++) { // parse through the board
        if (Grid[row][col].getValue() == 0) { // search for empty cells
          one_value = Grid[row][col].one_option(); // if the cell has only one possible value (it is also filled with this option if true)
          if (one_value) { // if the cell has only one option, we enter a loop to remove that value from the options of cells of the same row/column/minigrid
            for (int r = 0; r < 9; r++) {
              for (int c = 0; c < 9; c++) { // parse through entire grid
                if (Grid[row][col].getRow() == Grid[r][c].getRow()) // remove option from cells of the same row
                  Grid[r][c].removeOption(Grid[row][col].getValue());
                if (Grid[row][col].getColumn() == Grid[r][c].getColumn()) // remove option from cells of the same column
                  Grid[r][c].removeOption(Grid[row][col].getValue());
                if (Grid[row][col].getMiniGrid() == Grid[r][c].getMiniGrid()) // remove option from cells of the same minigrid
                  Grid[r][c].removeOption(Grid[row][col].getValue());
              }
            }
          }
          else { cnt += 1; } // increase cnt if a cell has more than one option
        }
        else { cnt += 1; } // increase cnt if a cell is already filled
      }
    }
    // Singletons Algorithm
    if (cnt >= 81) {
      for (int a = 0; a < 9; a++) {
        for (int b = 0; b < 9; b++) {
          // Check Rows for Singletons
          option_cnt = 0;
          for (int c = 0; c < 9; c++) {
            if (Grid[a][c].getValue() == 0) { // make sure the cells are empty
              if (Grid[a][c].check_option(b + 1)) { // We enter the if statement if the number is a possible option for this cell
                option_cnt += 1; // keeps track of how many times this value is a possible option in this row
                singleton = b + 1; // keeps track of this value in case of a singleton
                gridtracker = c; // keeps track of the column increase of a singleton
              }
            }
          }
          if (option_cnt == 1) { // if the count is equal to 1, that means we have a singleton
            Grid[a][gridtracker].singletonOptions(singleton); // remove other possible options (other than the singleton)
            Grid[a][gridtracker].setValue(singleton); // set the value of the cell to the singleton
            for (int r = 0; r < 9; r++) {
              for (int c = 0; c < 9; c++) { // parse the grid
                if (Grid[a][gridtracker].getRow() == Grid[r][c].getRow()) // if the cell is in the same row, remove the singleton from their options array
                  Grid[r][c].removeOption(Grid[a][gridtracker].getValue());
                if (Grid[a][gridtracker].getColumn() == Grid[r][c].getColumn()) // if the cell is in the same column, remove the singleton from options
                  Grid[r][c].removeOption(Grid[a][gridtracker].getValue());
                if (Grid[a][gridtracker].getMiniGrid() == Grid[r][c].getMiniGrid()) // if the cell is in the same minigrid, remove the singleton from options
                  Grid[r][c].removeOption(Grid[a][gridtracker].getValue());
              }
            }
          }
          // Columns (the same except the inner most for loop searches though rows and not columns)
          option_cnt = 0;
          for (int c = 0; c < 9; c++) {
            if (Grid[c][a].getValue() == 0) {
              if (Grid[c][a].check_option(b+1)) {
                option_cnt += 1;
                singleton = b + 1;
                gridtracker = c;
              }
            }
          }
          if (option_cnt == 1) {
            Grid[gridtracker][a].singletonOptions(singleton);
            Grid[gridtracker][a].setValue(singleton);
            for (int r = 0; r < 9; r++) {
              for (int c = 0; c < 9; c++) {
                if (Grid[gridtracker][a].getRow() == Grid[r][c].getRow())
                  Grid[r][c].removeOption(Grid[gridtracker][a].getValue());
                if (Grid[gridtracker][a].getColumn() == Grid[r][c].getColumn())
                  Grid[r][c].removeOption(Grid[gridtracker][a].getValue());
                if (Grid[gridtracker][a].getMiniGrid() == Grid[r][c].getMiniGrid())
                  Grid[r][c].removeOption(Grid[gridtracker][a].getValue());
              }
            }
          }
        }
      }
    }  // end Singletons Algorithm
  }
}
// Check if the entire puzzle has been solved 
// If every value is not 0, the puzzle is solved 
bool SudokuGrid::checkSolved() {
  bool finished;
  for (int row = 0; row < 9; row++) {
    for (int col = 0; col < 9; col++) {
      if (Grid[row][col].getValue() == 0) {
        finished = false;
        return finished;
      }
      else { finished = true; }
    }
  }
  return finished;
}
