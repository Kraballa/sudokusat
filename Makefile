all:
	g++ -c sudokusat.cpp -I"src/" -O3
	g++ sudokusat.o -o main -L"build/" -lcadical -lpsapi