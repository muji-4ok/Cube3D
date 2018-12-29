#pragma once
#include "CubeModel.h"
#include "Common.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <deque>
#include <utility>

class RotationQueue
{
public:
    RotationQueue() {};

    RotationHeader* pop();
    void push(RotationHeader* rh);
    void reset_angle();

    bool is_rotating() const;

    float get_last_angle() const;
    float get_angle() const;
    TempRotationHeader get_last_rotation() const;

private:
    std::deque<std::unique_ptr<RotationHeader>> queue;
    TempRotationHeader last_rotation;
    float angle = 0.0f;
};
