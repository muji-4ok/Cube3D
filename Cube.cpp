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

std::array<std::vector<float>*, 12> Cube::get_line_ptrs(const std::array<int, 3>& i_range, const std::array<int, 3>& j_range, const std::array<int, 3>& k_range)
{
    return std::array<std::vector<float>*, 12>();
}

template<typename T>
void append(std::vector<T>& v1, const std::vector<T>& v2)
{
    v1.insert(v1.end(), v2.begin(), v2.end());
}
