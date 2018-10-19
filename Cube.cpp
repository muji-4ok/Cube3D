#include "Cube.h"



Cube::Cube()
{
    std::vector<float> colors{
        1.0f, 1.0f, 0.0f, // yellow
        1.0f, 1.0f, 1.0f, // white
        1.0f, 0.0f, 0.0f, // red
        1.0f, 0.5f, 0.0f, // orange
        0.0f, 0.0f, 1.0f, // blue
        0.0f, 1.0f, 0.0f  // green
    };

    for (int i = 0; i < 3; ++i)
        for (int j = 0; j < 3; ++j)
            for (int k = 0; k < 3; ++k)
                cubes[i][j][k] = colors;
}


Cube::~Cube()
{
}

void Cube::rotate(char dir, bool clockwise)
{
    if (dir == 'U')
    {
        for (int i = 0; i < 3; ++i)
            for (int k = 0; k < 3; ++k)
                for (int coord = 0; coord < 6; ++coord)
                {
                    std::swap(cubes[i][2][k][2 * 3 + coord], cubes[i][2][k][1 * 3 + coord]);
                    std::swap(cubes[i][2][k][1 * 3 + coord], cubes[i][2][k][3 * 3 + coord]);
                    std::swap(cubes[i][2][k][3 * 3 + coord], cubes[i][2][k][0 * 3 + coord]);
                }
    }
    else if (dir == 'D')
    {
        for (int i = 0; i < 3; ++i)
            for (int k = 0; k < 3; ++k)
            {
                std::swap(cubes[i][0][k][2], cubes[i][0][k][1]);
                std::swap(cubes[i][0][k][1], cubes[i][0][k][3]);
                std::swap(cubes[i][0][k][3], cubes[i][0][k][0]);
            }
    }
    else if (dir == 'L')
    {

    }
    else if (dir == 'R')
    {

    }
    else if (dir == 'F')
    {

    }
    else if (dir == 'B')
    {

    }
}
