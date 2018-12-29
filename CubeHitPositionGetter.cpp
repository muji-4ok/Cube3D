#include "CubeHitPositionGetter.h"


bool InteractiveCubeHitPositionGetter::get_i(const glm::vec2 & mouse_pos, HitHeader & hit)
{
    auto world_ray = get_eye_ray(mouse_pos);
    auto mouse_origin = glm::vec3(0.0f, 0.0f, 0.0f);

    if (!hit_side(mouse_origin, world_ray, hit))
        return false;

    return true;
}

glm::vec3 InteractiveCubeHitPositionGetter::get_eye_ray(const glm::vec2 & mouse_pos)
{
    auto eye_ray = glm::inverse(cubeModel->projection) * glm::vec4(mouse_pos, -1.0f, 1.0f);
    eye_ray.z = -1.0f;
    eye_ray.w = 0.0f;

    return glm::normalize(glm::vec3(glm::inverse(cubeModel->translation_view) * eye_ray));
}

bool InteractiveCubeHitPositionGetter::hit_side(const glm::vec3 & mouse_origin,
                                                const glm::vec3 & world_ray, HitHeader & hit)
{
    bool is_anything_hit = false;
    float min_hit_dist = std::numeric_limits<float>::max();

    for (int i = 0; i < 3; ++i)
        for (int j = 0; j < 3; ++j)
            for (int k = 0; k < 3; ++k)
            {
                auto transform_matrix = cubeModel->view *
                    cubeModel->cubelets[i][j][k].rotation *
                    cubeModel->cubelets[i][j][k].full_translation;

                constexpr float epsilon = 1e-8;

                float min_dist = std::numeric_limits<float>::max();
                int min_index;
                bool is_hit = false;

                for (int v = 0; v < cubeModel->vertices.size(); v += 3)
                {
                    auto vertex1 = transform_matrix * glm::vec4(cubeModel->vertices[v], 1.0f);
                    auto vertex2 = transform_matrix * glm::vec4(cubeModel->vertices[v + 1], 1.0f);
                    auto vertex3 = transform_matrix * glm::vec4(cubeModel->vertices[v + 2], 1.0f);

                    float dist = dist_ray_to_triangle(mouse_origin, world_ray, vertex1, vertex2, vertex3);

                    // std::cout << dist << '\n';

                    bool non_zero = dist > epsilon;
                    int index = cubeModel->raw_vertices[v * 4 + 3];

                    if (non_zero && dist < min_dist)
                    {
                        is_hit = true;
                        min_dist = dist;
                        min_index = index;
                    }
                }

                if (is_hit && min_dist < min_hit_dist)
                {
                    min_hit_dist = min_dist;
                    hit.i = i;
                    hit.j = j;
                    hit.k = k;
                    hit.index = min_index;
                    is_anything_hit = true;
                }
            }

    return is_anything_hit;
}

float InteractiveCubeHitPositionGetter::dist_ray_to_triangle(const glm::vec3 & origin, const glm::vec3 & dir,
                                                             const glm::vec3 & v0, const glm::vec3 & v1,
                                                             const glm::vec3 & v2)
{
    auto e1 = v1 - v0;
    auto e2 = v2 - v0;

    auto p_vec = glm::cross(dir, e2);
    float det = glm::dot(e1, p_vec);

    constexpr float epsilon = 1e-8;

    if (det < epsilon && det > -epsilon)
        return 0.0f;

    float inv_det = 1.0f / det;
    auto t_vec = origin - v0;
    float u = glm::dot(t_vec, p_vec) * inv_det;

    if (u < 0 || u > 1)
        return 0.0f;

    auto q_vec = glm::cross(t_vec, e1);
    float v = glm::dot(dir, q_vec) * inv_det;

    if (v < 0 || u + v > 1)
        return 0.0f;

    return glm::dot(e2, q_vec) * inv_det;
}

HitHeader ScriptCubeHitPositionGetter::get_s(char r)
{
    HitHeader hit;

    if (r == 'U' || r == 'u')
    {
        hit.index = 1;
        hit.i = 0;
        hit.j = 2;
        hit.k = 2;
    }
    else if (r == 'D' || r == 'd')
    {
        hit.index = 1;
        hit.i = 0;
        hit.j = 0;
        hit.k = 2;
    }
    else if (r == 'L' || r == 'l')
    {
        hit.index = 1;
        hit.i = 0;
        hit.j = 0;
        hit.k = 2;
    }
    else if (r == 'R' || r == 'r')
    {
        hit.index = 1;
        hit.i = 2;
        hit.j = 0;
        hit.k = 2;
    }
    else if (r == 'F' || r == 'f')
    {
        hit.index = 3;
        hit.i = 2;
        hit.j = 0;
        hit.k = 2;
    }
    else if (r == 'B' || r == 'b')
    {
        hit.index = 3;
        hit.i = 2;
        hit.j = 0;
        hit.k = 0;
    }

    return hit;
}
