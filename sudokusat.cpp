#include <iostream>
#include <fstream>
#include <cstdio>
#include "cadical.hpp"

inline int getIndex(int x, int y, int i, int gridSize)
{
    return 1 + x + y * gridSize + i * gridSize * gridSize;
}

int getBlockNumber(int blockX, int blockY, int i, int blockIndex, int dim)
{
    int x = blockX * dim + blockIndex % dim;
    int y = blockY * dim + blockIndex / dim;
    return getIndex(x, y, i, dim * dim);
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
                int idx = getIndex(x, y, val - 1, gridSize);
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
                    std::cout << z + 1 << " ";
                }
            }
        }
        std::cout << std::endl;
    }
}

int generateFormula(CaDiCaL::Solver *solver, int dim)
{
    int gridSize = dim * dim;

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
        for (int blockX = 0; blockX < dim; blockX++)
        {
            for (int blockY = 0; blockY < dim; blockY++)
            {
                for (int blockIndex1 = 0; blockIndex1 < gridSize - 1; blockIndex1++)
                {
                    for(int blockIndex2 = blockIndex1 + 1; blockIndex2 < gridSize; blockIndex2++){
                        solver->add(-getBlockNumber(blockX,blockY,z,blockIndex1,dim));
                        solver->add(-getBlockNumber(blockX,blockY,z,blockIndex2,dim));
                        solver->add(0);
                    }
                }
            }
        }
    }

    /*
    //EXTENDED ENCODING NOT WORKING
    //Extended encoding
    // at most one number in each entry
    for (int x = 0; x < gridSize; x++)
    {
        for (int y = 0; y < gridSize; y++)
        {
            for (int z = 0; z < gridSize - 1; z++)
            {
                for (int i = z + 1; x < gridSize; i++)
                {
                    solver->add(-1 * getIndex(x, y, z, gridSize));
                    solver->add(-1 * getIndex(x, y, i, gridSize));
                    solver->add(0);
                }
            }
        }
    }

    // at least once in each row

    for (int y = 0; y < gridSize; y++)
    {
        for (int z = 0; z < gridSize; z++)
        {
            for (int x = 0; x < gridSize; x++)
            {
                solver->add(getIndex(x, y, z, gridSize));
            }
            solver->add(0);
        }
    }

    // at least once in each column

    for (int x = 0; x < gridSize; x++)
    {
        for (int z = 0; z < gridSize; z++)
        {
            for (int y = 0; y < gridSize; y++)
            {
                solver->add(getIndex(x, y, z, gridSize));
            }
            solver->add(0);
        }
    }
    

    /*
    // at least once in each 3x3 grid
    //TODO*/
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

    std::cout << "sudoku dimension: " << dim << std::endl;

    CaDiCaL::Solver *solver = new CaDiCaL::Solver;

    generateFormula(solver, dim);

    loadProblem(puzzle, solver, dim * dim);

    solver->simplify(3);
    std::cout << "simplified" << std::endl;
    int res = solver->solve();

    solver->statistics();

    if (res == 10)
    {
        std::cout << "sat" << std::endl;
        printSolution(solver, dim * dim);
    }
    else
    {
        std::cout << "unsat" << std::endl;
    }
    return 0;
}