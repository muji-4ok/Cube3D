#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <string>
#include <vector>


class Observer
{
public:
    virtual void update() = 0;
};

class Observable
{
public:
    void add_observer(Observer* observer);
    void notify();

private:
    std::vector<Observer*> observers;

};

enum RotationDir
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
    RotationDir dir;
    glm::vec2 dir_vec;
};

class HitModel
{
public:
    HitModel() {};

    const HitHeader& get_header() const;
    HitHeader&& pop_header();
    void set_header(const HitHeader& hit);
    void set_position(const HitHeader& hit);
    void set_dir(const HitHeader& hit);

    bool has_dir;
    bool has_position;

private:
    HitHeader hit;
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

struct SetHitRotationHeader : public RotationHeader
{
    SetHitRotationHeader() {};
    SetHitRotationHeader(const HitHeader &hit) : hit(hit) {}

    HitHeader hit;

    std::string get_name() override;
};

struct SetHitPosRotationHeader : public RotationHeader
{
    SetHitPosRotationHeader() {};
    SetHitPosRotationHeader(const HitHeader &hit) : hit(hit) {}

    HitHeader hit;

    std::string get_name() override;
};

struct SetHitDirRotationHeader : public RotationHeader
{
    SetHitDirRotationHeader() {};
    SetHitDirRotationHeader(const HitHeader &hit) : hit(hit) {}

    HitHeader hit;

    std::string get_name() override;
};

