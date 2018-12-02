#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

enum Rotation_Dir
{
    DIR1 = 0,
    DIR2 = 1
};

struct HitHeader
{
    // HitHeader(int in, int i, int j, int k, Rotation_Dir d) : index(in), i(i), j(j), k(k), dir(d) {}
    int index;
    int i;
    int j;
    int k;
    Rotation_Dir dir;
    glm::vec2 dir_vec;
};

struct RotationHeader
{
    glm::vec3 vec;
    float angle;
};

enum State
{
    Interactive = 0,
    Walkthrough = 1,
    Input = 2
};