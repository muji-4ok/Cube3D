#include "Cube.h"



Cube::Cube()
{
    std::vector<float> c{
        1.0f, 1.0f, 0.0f, // yellow
        1.0f, 1.0f, 1.0f, // white
        1.0f, 0.0f, 0.0f, // red
        1.0f, 0.5f, 0.0f, // orange
        0.0f, 0.0f, 1.0f, // blue
        0.0f, 1.0f, 0.0f  // green
    };

    std::vector<float> y{ 1.0f, 1.0f, 0.0f };
    std::vector<float> w{ 1.0f, 1.0f, 1.0f };
    std::vector<float> r{ 1.0f, 0.0f, 0.0f };
    std::vector<float> o{ 1.0f, 0.5f, 0.0f };
    std::vector<float> b{ 0.0f, 0.0f, 1.0f };
    std::vector<float> g{ 0.0f, 1.0f, 0.0f };
    std::vector<float> n{ 0.0f, 0.0f, 0.0f };

    for (int i = 0; i < 3; ++i)
        for (int j = 0; j < 3; ++j)
        {
            faces['U'][i * 3 + j] = 'g';
            faces['D'][i * 3 + j] = 'b';
            faces['R'][i * 3 + j] = 'o';
            faces['L'][i * 3 + j] = 'r';
            faces['F'][i * 3 + j] = 'w';
            faces['B'][i * 3 + j] = 'y';
        }

    for (int i = 0; i < 3; ++i)
        for (int j = 0; j < 3; ++j)
            for (int k = 0; k < 3; ++k)
            {
                append(cubes[i][j][k], k == 0 ? y : n);
                append(cubes[i][j][k], k == 2 ? w : n);
                append(cubes[i][j][k], i == 0 ? r : n);
                append(cubes[i][j][k], i == 2 ? o : n);
                append(cubes[i][j][k], j == 0 ? b : n);
                append(cubes[i][j][k], j == 2 ? g : n);
            }
}

Cube & Cube::operator=(const Cube & c)
{
    if (&c == this)
        return *this;

    faces = c.faces;
    cubes = c.cubes;
    models = c.models;
    models_full_size = c.models_full_size;

    return *this;
}

Cube & Cube::operator=(Cube && c)
{
    if (&c == this)
        return *this;

    faces = std::move(c.faces);
    cubes = std::move(c.cubes);
    models = std::move(c.models);
    models_full_size = std::move(c.models_full_size);

    return *this;
}


Cube::~Cube()
{
}

void Cube::rotate(char dir, bool clockwise)
{
    if (dir == 'U')
    {
        swap_3(&faces['L'][0], &faces['F'][0]);
        swap_3(&faces['F'][0], &faces['R'][0]);
        swap_3(&faces['R'][0], &faces['B'][0]);

        //swap_3(&faces['U'][0], &faces['U'][0]);
    }
    else if (dir == 'D')
    {

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

template <typename T>
void swap_3(T* first, T* second, int inc)
{
    for (int i = 0; i < 3; i += inc)
        std::swap(first[i], second[i]);
}

template<typename T>
void append(std::vector<T>& v1, const std::vector<T>& v2)
{
    v1.insert(v1.end(), v2.begin(), v2.end());
}
