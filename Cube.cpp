#include "Cube.h"

Cube::Cube()
{
    // std::vector<float> c{
        // 1.0f, 1.0f, 0.0f, // yellow
        // 1.0f, 1.0f, 1.0f, // white
        // 1.0f, 0.0f, 0.0f, // red
        // 1.0f, 0.5f, 0.0f, // orange
        // 0.0f, 0.0f, 1.0f, // blue
        // 0.0f, 1.0f, 0.0f  // green
    // };

    set_solved();
}

Cube & Cube::operator=(const Cube & c)
{
    if (&c == this)
        return *this;

    cubes = c.cubes;
    models = c.models;
    models_full_size = c.models_full_size;

    return *this;
}

Cube & Cube::operator=(Cube && c)
{
    if (&c == this)
        return *this;

    cubes = std::move(c.cubes);
    models = std::move(c.models);
    models_full_size = std::move(c.models_full_size);

    return *this;
}

Cube::~Cube()
{
}

void Cube::rotate(std::array<std::vector<float>*, 8> cubelets, CubeletRotation dir)
{
    for (auto &p : cubelets)
    {
        auto &c = *p;
        rotate_cubelet(c, dir);
    }

    for (int t = 0; t < 2; ++t)
        for (int i = 1; i < 8; ++i)
            std::swap(*cubelets[i - 1], *cubelets[i]);
}

void Cube::set_solved()
{
    std::vector<float> y{ 1.0f, 1.0f, 0.0f };
    std::vector<float> w{ 1.0f, 1.0f, 1.0f };
    std::vector<float> r{ 1.0f, 0.0f, 0.0f };
    std::vector<float> o{ 1.0f, 0.5f, 0.0f };
    std::vector<float> b{ 0.0f, 0.0f, 1.0f };
    std::vector<float> g{ 0.0f, 1.0f, 0.0f };
    std::vector<float> n{ 0.0f, 0.0f, 0.0f };

    for (int i = 0; i < 3; ++i)
        for (int j = 0; j < 3; ++j)
            for (int k = 0; k < 3; ++k)
            {
                cubes[i][j][k].clear();
                append(cubes[i][j][k], k == 0 ? y : n);
                append(cubes[i][j][k], k == 2 ? w : n);
                append(cubes[i][j][k], i == 0 ? r : n);
                append(cubes[i][j][k], i == 2 ? o : n);
                append(cubes[i][j][k], j == 0 ? b : n);
                append(cubes[i][j][k], j == 2 ? g : n);
            }
}

void Cube::rotate_cubelet(std::vector<float>& cubelet, CubeletRotation dir)
{
    switch (dir)
    {
        case X:
            swap_indices(cubelet, 1, 5);
            swap_indices(cubelet, 5, 0);
            swap_indices(cubelet, 0, 4);
            break;
        case Y:
            swap_indices(cubelet, 1, 3);
            swap_indices(cubelet, 3, 0);
            swap_indices(cubelet, 0, 2);
            break;
        case Z:
            swap_indices(cubelet, 2, 5);
            swap_indices(cubelet, 5, 3);
            swap_indices(cubelet, 3, 4);
            break;
    }
}

void Cube::swap_indices(std::vector<float>& cubelet, int i1, int i2)
{
    swap_3(cubelet, i1, i2);
}

template<typename T>
void append(std::vector<T>& v1, const std::vector<T>& v2)
{
    v1.insert(v1.end(), v2.begin(), v2.end());
}

template<typename T>
void swap_3(std::vector<T>& v, int offset1, int offset2)
{
    for (int i = 0; i < 3; ++i)
        std::swap(v[offset1 * 3 + i], v[offset2 * 3 + i]);
}
