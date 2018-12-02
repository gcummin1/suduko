#include "Cell.h" 
#include <array> 
#include <iostream>
using namespace std; 
// Constructor 
Cell::Cell() {
  // Set Options Array
  options = new int[9]; // This array contains all the possible values that can fill the cell
  for (int i = 0; i < 9; i++)
    options[i] = i + 1; // Initially 1-9, but can be changed later
}
// Deconstructor 
Cell::~Cell() { delete [] options; } 
// Set the value of the cell (0 indicates an empty cell) 
void Cell::setValue(int num) { value = num; } 
// Tell the cell what column it is a part of (0-8) 
void Cell::setColumn(int c) { column = c; } 
// Tell the cell what row it is a part of (0-8) 
void Cell::setRow(int r) { row = r; } 
// Tell the cell which minigrid it is a part of (1-9) 
void Cell::setMiniGrid(int grid) { minigrid = grid; } 
// Return the number inside the cell (0 indicates an empty cell) 
int Cell::getValue() { return value; } 
// Return the row number that the cell is a part of (0-8) 
int Cell::getRow() { return row; } 
// Return the column number that the cell is a part of (0-8) 
int Cell::getColumn() { return column; } 
// Return the minigrid number that the cell is a part of (1-9) 
int Cell::getMiniGrid() { return minigrid; } 
// Determines if there is only one possible value for the cell, and if so sets its value to it 
bool Cell::one_option() {
  int zero_counter = 0;
  int poss_value;
  bool final_option = false;
  for (int i = 0; i < 9; i++) { // count the number of zeros in the 'options' array
    if (options[i] == 0) { zero_counter += 1; } // a zero indicates that option cannot be used
    else { poss_value = options[i]; } // a number != 0 is saved as a possible value
  }
  if (zero_counter == 8) { // indicates that there are 8 numbers that value cannot be, meaning 1 number it can be
    value = poss_value; // set the value to the possible value
    final_option = true; // return true, there was only one option for this cell's value
  }
  return final_option;
}
// Remove an option from the 'options' array 
// The provided value can no longer be used (it is being used in the same column/row/minigrid) 
void Cell::removeOption(int opt) {
  options[opt - 1] = 0; // remove the option from the array
}
// Check to see if the provided number is an option for this cell 
bool Cell::check_option(int num) {
  if (options[num - 1] == 0) { return false; }
  else { return true; }
}
// Rather than using removeOption over and over, this function skips to the final 'options' array when a singleton is found 
// Only the one option remains in the options array 
void Cell::singletonOptions(int singleton) {
  for (int a = 0; a < 9; a++) {
    if (a == singleton - 1) { continue; }
    else { options[a] = 0; }
  }
}
