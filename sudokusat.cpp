#include <iostream>
#include <cstdio>

int getIndex(int x, int y, int i, int gridSize)
{
    return 1 + x + y * gridSize + i * gridSize * gridSize;
}

int main(char **args, int argv)
{

    if (argv != 2)
    {
        std::cout << "missing problem parameter" << std::endl;
        return -1;
    }

    std::FILE *file = fopen(args[0], "r");

    int dim = std::fscanf(file, "%d");
    static int GRID_SIZE = dim * dim;

    std::cout << "sudoku size: " << GRID_SIZE << std::endl;

    // generateFormula();
    return 0;
}

int generateFormula(int gridSize)
{

    //(x1 | x2) ^ (-x3 | x5)
    // solver.add(1)
    // solver.add(2)
    // solver.add(0)
    // solver.add(-3)
    // solver.add(5)
    // solver.add(0)

    /*for (int row = 0; row < gridSize; row++)
    {
        for (int col = 0; col < gridSize; col++)
        {
            for (int num; num < gridSize; num++)
            {
                for (int mul : {-1, 1})
                {
                    int currIndex = getIndex(row, col, num, gridSize);
                    for (int rowCnt = row; rowCnt < gridSize; rowCnt++)
                    {
                        solver.add(currIndex);
                        solver.add(getIndex(rowCnt, col, num, gridSize));
                    }
                    for (int colCnt = col; colCnt < gridSize; colCnt++)
                    {

                    }
                    for (int boxCnt = 0; boxCnt < gridSize; boxCnt++)
                    {

                    }
                }
            }
        }
    }*/
}

// load in the given numbers of the problem
int loadProblem(std::FILE *file, int gridSize)
{
    int count = 0;
    for (int y = 0; y < gridSize; y++)
    {
        for (int x = 0; x < gridSize; x++)
        {
            int val = std::fscanf(file, "%d");
            if (val != 0)
            {
                int idx = getIndex(x, y, val, gridSize);
                // assert idx as unit literal
                //solver.add(idx);
                //solver.add(0);
                count++;
            }
        }
        std::fscanf(file, "\n");
    }
    std::cout << "inserted " << count << " given numbers" << std::endl;
}
