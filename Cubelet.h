#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <vector>
#include <utility>
#include <algorithm>
#include <map>


enum RotationAxis
{
    X = 0,
    Y = 1,
    Z = 2
};

enum SideColor
{
    Yellow = 0,
    White = 1,
    Red = 2,
    Orange = 3,
    Blue = 4,
    Green = 5,
    None = 6
};

struct ColorUtil
{
    static char toChar(SideColor c);
    static SideColor toEnum(char c);
    static glm::vec3 toGlmVec(SideColor c);
    static std::vector<float> toStdVec(SideColor c);
    static SideColor guessColor(const glm::vec3& colorVec);
    static glm::vec3 normalizedColor(const glm::vec3& c);
    static float getHue(const glm::vec3& c);
};

class Cubelet
{
public:
    Cubelet() noexcept : i(-1), j(-1), k(-1) {};
    Cubelet(int i, int j, int k) noexcept;

    void reset_colors();
    void reset_rotation();
    void reset_translation();

    void rotate(RotationAxis rot);
    void setFaceColor(int index, SideColor c);

    std::vector<float> colors;
    glm::mat4 rotation;
    glm::mat4 translation;
    glm::mat4 full_translation;
    int i;
    int j;
    int k;
};

template <typename T>
void append(std::vector<T>& v1, const std::vector<T>& v2);

template<typename T>
void swap_3(std::vector<T>& v, int offset1, int offset2);
