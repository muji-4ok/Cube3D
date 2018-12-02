#pragma once
#include "CubeModel.h"
#include "Common.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <queue>
#include <utility>

class RotationQueue
{
public:
    RotationQueue();

    bool is_rotating();
    RotationHeader&& pop();
    void push(RotationHeader&& rh);
    HitHeader& get_hit_header();
    void set_hit_header(HitHeader&& hh);
    void set_dir(Rotation_Dir dir, glm::vec2 dir_vec);
    bool has_dir();

private:
    std::queue<RotationHeader> queue;
    HitHeader hit_header;
    bool is_with_dir = false;

};

