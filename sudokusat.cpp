#include <iostream>
#include <cstdio>
#include "cadical.hpp"

int getIndex(int x, int y, int i, int gridSize)
{
    return 1 + x + y * gridSize + i * gridSize * gridSize;
}

// load in the given numbers of the problem
void loadProblem(std::FILE *file, CaDiCaL::Solver* solver, int gridSize)
{
    int count = 0;
    for (int y = 0; y < gridSize; y++)
    {
        for (int x = 0; x < gridSize; x++)
        {
            int* val;
            std::fscanf(file, "%d", val);
            if (val != 0)
            {
                int idx = getIndex(x, y, *val, gridSize);
                // assert idx as unit literal
                solver->assume(idx);
                count++;
            }
        }
        std::fscanf(file, "\n");
    }
    std::cout << "inserted " << count << " given numbers" << std::endl;
}

int main(int argv, char **args)
{

    if (argv != 2)
    {
        std::cout << "missing problem parameter" << std::endl;
        return -1;
    }

    std::FILE *file = fopen(args[0], "r");

    int* dim;
    std::fscanf(file, "%d", dim);
    static int GRID_SIZE = *dim * *dim;

    std::cout << "sudoku size: " << GRID_SIZE << std::endl;

    CaDiCaL::Solver* solver = new CaDiCaL::Solver;

    // generateFormula();

    loadProblem(file, solver, GRID_SIZE);

    int res = solver->solve();
    std::cout << "solution: " << res << std::endl;
    return 0;
}

int generateFormula(int dim)
{
    int gridSize = dim * dim;
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

    // basic encoding

    // at least one number in each entry
    
    for (int x = 0; x < gridSize; x++) {
        for (int y = 0; y < gridSize; y++) {
            for (int z = 0; z < gridSize; z++) {
                solver->add(getIndex(x, y, z, gridSize));
            }
            solver->add(0);   
        }   
    }
    solver->add(0);

    // each number at most once in each row

    for (int y = 0; y < gridSize; y++) {
        for (int z = 0; z < gridSize; z++) {
            for (int x = 0; x < gridSize - 1; x++) {
                for(int i = x+1; i < gridSize; i++) {
                    solver->add(-1 * getIndex(x, y, z, gridSize));
                    solver->add(-1 * getIndex(i, y, z, gridSize));
                    solver->add(0);
                }
            } 
        }   
    }
    solver->add(0);

    // at most once in each column

    for (int x = 0; x < gridSize; x++) {
        for (int z = 0; z < gridSize; z++) {
            for (int y = 0; y < gridSize - 1; y++) {
                for(int i = y+1; i < gridSize; i++) {
                    solver->add(-1 * getIndex(x, y, z, gridSize));
                    solver->add(-1 * getIndex(x, i, z, gridSize));
                    solver->add(0);
                }
            } 
        }   
    }
    solver->add(0);

    // at most once in each 3x3

    for (int z = 0; z < gridSize; z++) {
        for (int i = 0; i < dim; i++) {
            for(int j = 0; j < dim; j++) {
                for(int x = 0; x > dim; x++) {
                    for(int y = 0; y < dim; y++) {
                        for(int k = y+1; k < dim; k++) {
                            solver->add(-1*getIndex(3*i+x, 3*j+y, z, gridSize));
                            solver->add(-1*getIndex(3*i+x, 3*j+k, z, gridSize));
                            solver->add(0);
                        }
                    }
                }    
            }
        }
    }
    solver->add(0);

    for (int z = 0; z < gridSize; z++) {
        for (int i = 0; i < dim; i++) {
            for(int j = 0; j < dim; j++) {
                for(int x = 0; x > dim; x++) {
                    for(int y = 0; y < dim; y++) {
                        for(int k = y+1; k < dim; k++) {
                            for(int l = 0; l < dim; l++) {
                            solver->add(-1*getIndex(3*i+x, 3*j+y, z, gridSize));
                            solver->add(-1*getIndex(3*i+k, 3*j+l, z, gridSize));
                            solver->add(0);
                            }
                        }
                    }
                }    
            }
        }
    }
    solver->add(0);

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
