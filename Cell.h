// Cell.h
#ifndef CELL_H 
#define CELL_H 
#include <iostream>
class Cell {
  public:
    Cell(); // Constructor
    ~Cell(); // Destructor
    void setValue(int); // Set the value of the cell on the board
    void setColumn(int); // Set the value of the column on the board
    void setRow(int); // Set the value of the row on the board
    void setMiniGrid(int); // Set the value of the minigrid on the board
    int getValue(); // Return the value of the cell
    int getRow(); // Return which row of the board the cell is in
    int getColumn(); // Return which column of the board the cell is in
    int getMiniGrid(); // Return which minigrid on the board the cell is in
    bool one_option(); // Determines if there is only one possible value for the cell and sets the value to it
    void removeOption(int); // Remove an option (by setting it to 0) in the options array
    bool check_option(int); // Check to see if the provided number is an option for this cell
    void singletonOptions(int); // Removes all options in the options array except for the provided value
  private:
    int value; // the number that the cell represtents
    int row; // The row that the cell is contained in
    int column; // the column that the cell is contained in
    int minigrid; // The minigrid that the cell is contained in
    int * options; // An array of the possible values that an empty cell can take on
};
#endif
