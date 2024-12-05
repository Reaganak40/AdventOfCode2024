#include "pch.h"

int DoCrosswordPuzzle(const std::vector<std::string>& grid)
{
    auto check_for_xmas = [](const std::vector<std::string>& grid, int x, int y) {
        std::string word = "";
        int total = 0;
        
        // check right
        if (x + 3 < grid[y].size())
        {
            word += grid[y][x];
            word += grid[y][x + 1];
            word += grid[y][x + 2];
            word += grid[y][x + 3];

            if (word == "XMAS")
            {
                total++;
            }
            word.clear();
        }

        // check left
        if (x - 3 >= 0)
        {
            word += grid[y][x];
            word += grid[y][x - 1];
            word += grid[y][x - 2];
            word += grid[y][x - 3];

            if (word == "XMAS")
            {
                total++;
            }
            word.clear();
        }

        // check down
        if (y + 3 < grid.size())
        {
            word += grid[y][x];
            word += grid[y + 1][x];
            word += grid[y + 2][x];
            word += grid[y + 3][x];

            if (word == "XMAS")
            {
                total++;
            }
            word.clear();
        }

        // check up
        if (y - 3 >= 0)
        {
            word += grid[y][x];
            word += grid[y - 1][x];
            word += grid[y - 2][x];
            word += grid[y - 3][x];

            if (word == "XMAS")
            {
                total++;
            }
            word.clear();
        }

        // check top right diagonal
        if (x + 3 < grid[y].size() && y - 3 >= 0)
        {
            word += grid[y][x];
            word += grid[y - 1][x + 1];
            word += grid[y - 2][x + 2];
            word += grid[y - 3][x + 3];

            if (word == "XMAS")
            {
                total++;
            }
            word.clear();
        }

        // check top left diagonal
        if (x - 3 >= 0 && y - 3 >= 0)
        {
            word += grid[y][x];
            word += grid[y - 1][x - 1];
            word += grid[y - 2][x - 2];
            word += grid[y - 3][x - 3];

            if (word == "XMAS")
            {
                total++;
            }
            word.clear();
        }

        // check bottom right diagonal
        if (x + 3 < grid[y].size() && y + 3 < grid.size())
        {
            word += grid[y][x];
            word += grid[y + 1][x + 1];
            word += grid[y + 2][x + 2];
            word += grid[y + 3][x + 3];

            if (word == "XMAS")
            {
                total++;
            }
            word.clear();
        }

        // check bottom left diagonal
        if (x - 3 >= 0 && y + 3 < grid.size())
        {
            word += grid[y][x];
            word += grid[y + 1][x - 1];
            word += grid[y + 2][x - 2];
            word += grid[y + 3][x - 3];

            if (word == "XMAS")
            {
                total++;
            }
            word.clear();
        }

        return total;
    };

    int total = 0;
    for (int i = 0; i < grid.size(); i++)
    {
        for (int j = 0; j < grid[i].size(); j++)
        {
            total += check_for_xmas(grid, j, i);
        }
    }

    return total;
}

int DoCrosswordPuzzle2(const std::vector<std::string>& grid)
{
    auto check_for_xmas = [](const std::vector<std::string>& grid, int x, int y) {
        std::string word = "";
        
        if (grid[y][x] != 'A') {
            return false;
        }

        // check diagonal 1
        if (x + 1 < grid[y].size() && y + 1 < grid.size())
        {
            if (x - 1 >= 0 && y - 1 >= 0)
            {
                word += grid[y-1][x-1];
                word += grid[y][x];
                word += grid[y+1][x+1];

                if (word != "MAS" && word != "SAM")
                {
                    return false;
                }
                word.clear();

            } else {
                return false;
            }
        } else {
            return false;
        }

        // check diagonal 2
        if (x - 1 >= 0 && y + 1 < grid.size())
        {
            if (x + 1 < grid[y].size() && y - 1 >= 0)
            {
                word += grid[y-1][x+1];
                word += grid[y][x];
                word += grid[y+1][x-1];

                if (word != "MAS" && word != "SAM")
                {
                    return false;
                }
                word.clear();

            } else {
                return false;
            }
        } else {
            return false;
        }

        return true;
    };

    int total = 0;
    for (int i = 0; i < grid.size(); i++)
    {
        for (int j = 0; j < grid[i].size(); j++)
        {
            if(check_for_xmas(grid, j, i)) {
                total++;
            }
        }
    }

    return total;
}


void DoDay4()
{
    std::cout << "Day 4" << std::endl;
    std::ifstream file("inputs/day4/input.txt");
    if (!file.is_open())
    {
        std::cerr << "Could not open file" << std::endl;
        return;
    }

    std::string line;
    std::vector<std::string> grid;
    while (std::getline(file, line))
    {
        grid.push_back(line);
    }

    std::cout << "    Total XMAS found: " << DoCrosswordPuzzle(grid) << std::endl;
    std::cout << "    Total 'X-MAS' found: " << DoCrosswordPuzzle2(grid) << std::endl;
}