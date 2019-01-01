#include "Cubelet.h"


Cubelet::Cubelet(int i, int j, int k) noexcept : i(i), j(j), k(k) 
{
    reset_colors();
}

void Cubelet::reset_colors()
{
    static std::vector<float> y{ 1.0f, 1.0f, 0.0f };
    static std::vector<float> w{ 1.0f, 1.0f, 1.0f };
    static std::vector<float> r{ 1.0f, 0.0f, 0.0f };
    static std::vector<float> o{ 1.0f, 0.5f, 0.0f };
    static std::vector<float> b{ 0.0f, 0.0f, 1.0f };
    static std::vector<float> g{ 0.0f, 1.0f, 0.0f };
    static std::vector<float> n{ 0.0f, 0.0f, 0.0f };

    colors.clear();
    append(colors, k == 0 ? y : n);
    append(colors, k == 2 ? w : n);
    append(colors, i == 0 ? r : n);
    append(colors, i == 2 ? o : n);
    append(colors, j == 0 ? b : n);
    append(colors, j == 2 ? g : n);
}

void Cubelet::reset_rotation()
{
    translation = glm::mat4(1.0f);
}

void Cubelet::reset_translation()
{
    translation = glm::translate(glm::mat4(1.0f), glm::vec3(0.35f * (i - 1), 0.35f * (j - 1), 0.35f * (k - 1)));

    full_translation = glm::scale(full_translation, glm::vec3(0.35f));
    translation = glm::scale(translation, glm::vec3(1.0f / 3.0f));
}

void Cubelet::rotate(CubeletRotation rot)
{
    switch (rot)
    {
        case X:
            swap_3(colors, 1, 5);
            swap_3(colors, 5, 0);
            swap_3(colors, 0, 4);
            break;
        case Y:
            swap_3(colors, 1, 3);
            swap_3(colors, 3, 0);
            swap_3(colors, 0, 2);
            break;
        case Z:
            swap_3(colors, 2, 5);
            swap_3(colors, 5, 3);
            swap_3(colors, 3, 4);
            break;
    }
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
