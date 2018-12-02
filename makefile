PuzzleSolver: SolverMain.o Cell.o SudokuGrid.o
	g++ -o PuzzleSolver SolverMain.o Cell.o SudokuGrid.o 
SolverMain.o: SolverMain.cpp
	g++ -c -std=c++11 SolverMain.cpp 
SudokuGrid.o: SudokuGrid.cpp SudokuGrid.h
	g++ -c -std=c++11 SudokuGrid.cpp 
Cell.o: Cell.cpp Cell.h
	g++ -c -std=c++11 Cell.cpp 
clean:
	rm PuzzleSolver *.o
