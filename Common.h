#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <string>

enum CubeletRotation
{
    X = 0,
    Y = 1,
    Z = 2
};

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
    virtual std::string get_name() = 0;
};

struct ResetRotationHeader : public RotationHeader
{
    ResetRotationHeader() : reset(true) {}

    bool reset = true;
    std::string get_name() override;
};

struct TempRotationHeader : public RotationHeader
{
    TempRotationHeader() {};
    TempRotationHeader(const glm::vec3 &vec, float angle) : vec(vec), angle(angle) {}

    glm::vec3 vec = glm::vec3(1.0f);
    float angle = 0.0f;

    std::string get_name() override;
};

struct PermRotationHeader : public RotationHeader
{
    PermRotationHeader() {};
    PermRotationHeader(const HitHeader &hit, int turns) : hit(hit), turns(turns) {}

    HitHeader hit;
    int turns;

    std::string get_name() override;
};

enum State
{
    Interactive = 0,
    Walkthrough = 1,
    Input = 2
};