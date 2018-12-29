#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <vector>
#include <utility>


enum CubeletRotation
{
    X = 0,
    Y = 1,
    Z = 2
};

class Cubelet
{
public:
    Cubelet(int i, int j, int k);

    void reset_colors();
    void reset_rotation();
    void reset_translation();

    void rotate(CubeletRotation rot);

    std::vector<float> colors;
    glm::mat4 rotation;
    glm::mat4 translation;
    glm::mat4 full_translation;
    const int i;
    const int j;
    const int k;

private:
};

template <typename T>
void append(std::vector<T>& v1, const std::vector<T>& v2);

template<typename T>
void swap_3(std::vector<T>& v, int offset1, int offset2);
