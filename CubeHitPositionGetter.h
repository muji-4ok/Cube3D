#pragma once
#include "Common.h"
#include "Events.h"
#include "CubeModel.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>


class CubeHitPositionGetter
{
public:
    CubeHitPositionGetter(CubeModel* cm) : cubeModel(cm) {}

protected:
    CubeModel* cubeModel;

};

class InteractiveCubeHitPositionGetter : public CubeHitPositionGetter
{
public:
    InteractiveCubeHitPositionGetter(CubeModel* cm) : CubeHitPositionGetter(cm) {};

    bool get_i(const glm::vec2& mouse_pos, HitHeader& hit);

private:
    glm::vec3 get_eye_ray(const glm::vec2& mouse_pos);
    bool hit_side(const glm::vec3& mouse_origin, const glm::vec3& world_ray, HitHeader& hit);
    float dist_ray_to_triangle(const glm::vec3& origin, const glm::vec3& dir,
                               const glm::vec3& v0, const glm::vec3& v1, const glm::vec3& v2);

};

class ScriptCubeHitPositionGetter : public CubeHitPositionGetter
{
public:
    ScriptCubeHitPositionGetter(CubeModel* cm) : CubeHitPositionGetter(cm) {};

    HitHeader get_s(char r);

private:
};

class HybridCubeHitPositionGetter : public InteractiveCubeHitPositionGetter, public ScriptCubeHitPositionGetter
{
public:
    HybridCubeHitPositionGetter(CubeModel* cm) :
        InteractiveCubeHitPositionGetter(cm), ScriptCubeHitPositionGetter(cm) {};
};

