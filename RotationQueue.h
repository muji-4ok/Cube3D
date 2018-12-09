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
    RotationQueue();

    bool is_rotating();
    std::unique_ptr<RotationHeader> pop();
    void push(std::unique_ptr<RotationHeader> rh);
    HitHeader& get_hit_header();
    void set_hit_header(HitHeader&& hh);
    void set_dir(Rotation_Dir dir, glm::vec2 dir_vec);
    void reset();
    bool has_dir();
    bool has_header();
    float get_last_angle();
    float get_angle();
    TempRotationHeader get_last_rotation();

private:
    std::deque<std::unique_ptr<RotationHeader>> queue;
    TempRotationHeader last_rotation;
    HitHeader hit_header;
    float angle = 0.0f;
    bool is_with_dir = false;
    bool is_with_header = false;

};

