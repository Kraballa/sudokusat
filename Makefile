all:
	g++ -c sudokusat.cpp -I"cadical/" -O3 -Wall
	g++ sudokusat.o -o main -L"build/" -lcadical