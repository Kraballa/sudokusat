#include <iostream>
#include <fstream>
#include <cstdio>
#include "cadical.hpp"

inline int getIndex(int x, int y, int i, int gridSize)
{
    return 1 + x + y * gridSize + i * gridSize * gridSize;
}

// load in the given numbers of the problem
void loadProblem(std::fstream &file, CaDiCaL::Solver *solver, int gridSize)
{
    int count = 0;
    for (int y = 0; y < gridSize; y++)
    {
        for (int x = 0; x < gridSize; x++)
        {
            int val;
            file >> val;
            if (val != 0)
            {
                int idx = getIndex(x, y, val-1, gridSize);
                // assert idx as unit literal
                solver->add(idx);
                solver->add(0);
                count++;
            }
        }
    }
    std::cout << "inserted " << count << " given numbers" << std::endl;
}

void printSolution(CaDiCaL::Solver *solver, int gridSize)
{

    for (int y = 0; y < gridSize; y++)
    {
        for (int x = 0; x < gridSize; x++)
        {
            for (int z = 0; z < gridSize; z++)
            {
                if (solver->val(getIndex(x, y, z, gridSize)) > 0)
                {
                    std::cout << z+1 << " ";
                    break;
                }
            }
        }
        std::cout << std::endl;
    }
}

int generateFormula(CaDiCaL::Solver *solver, int dim)
{
    int gridSize = dim * dim;

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

    // basic encoding

    // at least one number in each entry

    for (int x = 0; x < gridSize; x++)
    {
        for (int y = 0; y < gridSize; y++)
        {
            for (int z = 0; z < gridSize; z++)
            {
                solver->add(getIndex(x, y, z, gridSize));
            }
            solver->add(0);
        }
    }

    // each number at most once in each row

    for (int y = 0; y < gridSize; y++)
    {
        for (int z = 0; z < gridSize; z++)
        {
            for (int x = 0; x < gridSize - 1; x++)
            {
                for (int i = x + 1; i < gridSize; i++)
                {
                    solver->add(-1 * getIndex(x, y, z, gridSize));
                    solver->add(-1 * getIndex(i, y, z, gridSize));
                    solver->add(0);
                }
            }
        }
    }

    // at most once in each column

    for (int x = 0; x < gridSize; x++)
    {
        for (int z = 0; z < gridSize; z++)
        {
            for (int y = 0; y < gridSize - 1; y++)
            {
                for (int i = y + 1; i < gridSize; i++)
                {
                    solver->add(-1 * getIndex(x, y, z, gridSize));
                    solver->add(-1 * getIndex(x, i, z, gridSize));
                    solver->add(0);
                }
            }
        }
    }

    // at most once in each 3x3

    for (int z = 0; z < gridSize; z++)
    {
        for (int i = 0; i < dim; i++)
        {
            for (int j = 0; j < dim; j++)
            {
                for (int x = 0; x > dim; x++)
                {
                    for (int y = 0; y < dim; y++)
                    {
                        for (int k = y + 1; k < dim; k++)
                        {
                            solver->add(-1 * getIndex(3 * i + x, 3 * j + y, z, gridSize));
                            solver->add(-1 * getIndex(3 * i + x, 3 * j + k, z, gridSize));
                            solver->add(0);
                        }
                    }
                }
            }
        }
    }

    for (int z = 0; z < gridSize; z++)
    {
        for (int i = 0; i < dim; i++)
        {
            for (int j = 0; j < dim; j++)
            {
                for (int x = 0; x > dim; x++)
                {
                    for (int y = 0; y < dim; y++)
                    {
                        for (int k = y + 1; k < dim; k++)
                        {
                            for (int l = 0; l < dim; l++)
                            {
                                solver->add(-1 * getIndex(3 * i + x, 3 * j + y, z, gridSize));
                                solver->add(-1 * getIndex(3 * i + k, 3 * j + l, z, gridSize));
                                solver->add(0);
                            }
                        }
                    }
                }
            }
        }
    }

    /*     // advanced encoding
        // at most one number in each entry
        for (int x = 0; x < gridSize; x++) {
            for (int y = 0; y < gridSize; y++) {
                for (int z = 0; z < gridSize - 1; z++) {
                    for (int i = z + 1; x < gridSize; i++) {
                        solver->add(-1 * getIndex(x, y, z, gridSize));
                        solver->add(-1 * getIndex(x, y, i, gridSize));
                        solver->add(0);
                    }
                }
            }
        }
        solver->add(0);

        // at least once in each row


        for (int y = 0; y < gridSize; y++) {
            for (int z = 0; z < gridSize; z++) {
                for (int x = 0; x < gridSize; x++) {
                    solver->add(getIndex(x, y, z, gridSize));
                }
               solver->add(0);
            }
        }
        solver->add(0);

        // at least once in each column

        for (int x = 0; x < gridSize; x++) {
            for (int z = 0; z < gridSize; z++) {
                for (int y = 0; y < gridSize; y++) {
                    solver->add(getIndex(x, y, z, gridSize));
                }
               solver->add(0);
            }
        }
        solver->add(0);
        // at least once in each 3x3 grid

        for (int z = 0; z < gridSize; z++) {
            // TODO
        } */
}

int main(int argc, char **argv)
{
    if (argc != 2)
    {
        std::cout << "missing problem parameter" << std::endl;
        return -1;
    }

    std::fstream puzzle(argv[1], std::ios_base::in);

    int dim;
    puzzle >> dim;

    int GRID_SIZE = dim * dim;

    std::cout << "sudoku size: " << GRID_SIZE << std::endl;

    CaDiCaL::Solver *solver = new CaDiCaL::Solver;

    generateFormula(solver, dim);

    loadProblem(puzzle, solver, GRID_SIZE);

    solver->simplify(3);
    std::cout << "simplified" << std::endl;
    int res = solver->solve();

    solver->statistics();

    std::cout << "solution: " << res << std::endl;
    printSolution(solver, GRID_SIZE);
    return 0;
}