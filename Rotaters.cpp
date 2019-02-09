#include "Rotaters.h"


void Rotater::process_queue()
{
    if (cubeModel->rotationQueue.is_rotating())
    {
        auto rot_header = cubeModel->rotationQueue.pop();

        auto temp_ptr = dynamic_cast<TempRotationHeader*>(rot_header);
        auto perm_ptr = dynamic_cast<PermRotationHeader*>(rot_header);
        auto reset_ptr = dynamic_cast<ResetRotationHeader*>(rot_header);
        auto set_hit_ptr = dynamic_cast<SetHitRotationHeader*>(rot_header);
        auto set_hit_pos_ptr = dynamic_cast<SetHitPosRotationHeader*>(rot_header);
        auto set_hit_dir_ptr = dynamic_cast<SetHitDirRotationHeader*>(rot_header);
        auto whole_rotation_ptr = dynamic_cast<WholeRotationHeader*>(rot_header);

        if (temp_ptr)
            process_temp(temp_ptr);
        else if (perm_ptr)
            process_perm(perm_ptr);
        else if (set_hit_ptr)
            process_hit(set_hit_ptr);
        else if (set_hit_pos_ptr)
            process_hit_pos(set_hit_pos_ptr);
        else if (set_hit_dir_ptr)
            process_hit_dir(set_hit_dir_ptr);
        else if (whole_rotation_ptr)
            process_whole_rotation(whole_rotation_ptr);
        else if (reset_ptr)
            ;
        else
            throw std::runtime_error("Incorrect pointer");
    }
}

void Rotater::process_temp(const TempRotationHeader* header)
{
    for (int i = 0; i < 3; ++i)
        for (int j = 0; j < 3; ++j)
            for (int k = 0; k < 3; ++k)
                if (needs_rotation(i, j, k))
                    cubeModel->cubelets[i][j][k].rotation =
                      glm::rotate(cubeModel->cubelets[i][j][k].rotation,
                                  header->angle,
                                  header->vec);
}

void Rotater::process_perm(const PermRotationHeader* header)
{
    auto cubelets = get_rotating_cubelets();
    auto rot_index = get_rot_index(header->hit.index, header->hit.dir);
    auto rotation_dir = get_cubelet_rotation(rot_index);

    for (int i = 0; i < header->turns; ++i)
    {
        for (auto &p : cubelets)
            p->rotate(rotation_dir);

        for (int t = 0; t < 2; ++t)
            for (int j = 1; j < 8; ++j)
                std::swap(cubelets[j - 1]->colors, cubelets[j]->colors);
    }

    cubeModel->hitModel.pop_header();
    cubeModel->reset_rotations();
    cubeModel->rotationQueue.reset_angle();
}

std::array<Cubelet*, 8> Rotater::get_rotating_cubelets() const
{
    using Coord = std::tuple<int, int, int>;
    std::array<Coord, 8> coords;

    auto count_centers = [](int i, int j, int k) -> int {
        return static_cast<int>(i == 1) + static_cast<int>(j == 1) +
                 static_cast<int>(k == 1);
    };

    bool is_center = needs_rotation(1, 1, 1);
    int c = 0;

    for (int i = 0; i < 3; ++i)
        for (int j = 0; j < 3; ++j)
            for (int k = 0; k < 3; ++k)
                if (needs_rotation(i, j, k))
                {
                    auto centers = count_centers(i, j, k);

                    if (is_center)
                    {
                        if (count_centers(i, j, k) == 3)
                            continue;
                    }
                    else
                    {
                        if (count_centers(i, j, k) >= 2)
                            continue;
                    }

                    coords[c++] = { i, j, k };
                }

    int rot_index = get_rot_index(cubeModel->hitModel.get_header().index,
                                  cubeModel->hitModel.get_header().dir);

    auto cmp = [rot_index](const Coord &c1, const Coord &c2) -> bool {
        switch (rot_index)
        {
            case 0:
            case 1:
                return glm::cross(glm::vec3(std::get<0>(c1) - 1, std::get<1>(c1) - 1, 0),
                                  glm::vec3(std::get<0>(c2) - 1, std::get<1>(c2) - 1, 0)).z < 0;
            case 2:
            case 3:
                return glm::cross(glm::vec3(std::get<1>(c1) - 1, std::get<2>(c1) - 1, 0),
                                  glm::vec3(std::get<1>(c2) - 1, std::get<2>(c2) - 1, 0)).z < 0;
            case 4:
            case 5:
                return glm::cross(glm::vec3(std::get<0>(c1) - 1, std::get<2>(c1) - 1, 0),
                                  glm::vec3(std::get<0>(c2) - 1, std::get<2>(c2) - 1, 0)).z < 0;
        }
    };

    std::sort(coords.begin(), coords.end(), cmp);

    std::array<Cubelet*, 8> out;

    for (int i = 0; i < 8; ++i)
        out[i] = &cubeModel->cubelets[std::get<0>(coords[i])]
                                     [std::get<1>(coords[i])]
                                     [std::get<2>(coords[i])];

    return out;
}

RotationAxis Rotater::get_cubelet_rotation(int rot_index) const
{
    switch (rot_index)
    {
        case 0:
        case 1:
            return Z;
        case 2:
        case 3:
            return X;
        case 4:
        case 5:
            return Y;
        default:
            throw std::runtime_error("Incorrect rot_index");
    }
}

bool Rotater::needs_rotation(int i, int j, int k) const
{
    auto& hit = cubeModel->hitModel.get_header();

    switch (hit.index)
    {
        case 0:
            if (hit.dir == DIR1)
                return j == hit.j;
            else
                return i == hit.i;
        case 1:
            if (hit.dir == DIR1)
                return j == hit.j;
            else
                return i == hit.i;
        case 2:
            if (hit.dir == DIR1)
                return j == hit.j;
            else
                return k == hit.k;
        case 3:
            if (hit.dir == DIR1)
                return j == hit.j;
            else
                return k == hit.k;
        case 4:
            if (hit.dir == DIR1)
                return k == hit.k;
            else
                return i == hit.i;
        case 5:
            if (hit.dir == DIR1)
                return k == hit.k;
            else
                return i == hit.i;
        default:
            throw std::runtime_error("Incorrect index");
    }
}

void Rotater::process_hit(const SetHitRotationHeader* header)
{
    cubeModel->hitModel.set_header(header->hit);
}

void Rotater::process_hit_pos(const SetHitPosRotationHeader* header)
{
    cubeModel->hitModel.set_position(header->hit);
}

void Rotater::process_hit_dir(const SetHitDirRotationHeader* header)
{
    cubeModel->hitModel.set_dir(header->hit);
}

void Rotater::process_whole_rotation(const WholeRotationHeader * header)
{
    constexpr float epsilon = 1e-3;

    if (std::abs(header->angle) > epsilon)
    {
        auto immediate_rotation = glm::rotate(glm::mat4(1.0f), header->angle, header->vec);
        cubeModel->rotation_view *= immediate_rotation;
        cubeModel->view *= immediate_rotation;
    }
}

HitHeader ScriptRotater::get_hit_header(char r) const
{
    HitHeader hit;
    get_hit_pos(r, hit);
    get_hit_dir(r, hit);

    return hit;
}

void ScriptRotater::get_hit_pos(char r, HitHeader& hit) const
{
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
}

void ScriptRotater::get_hit_dir(char r, HitHeader& hit) const
{
    glm::vec3 pane_vert_1, pane_vert_2, pane_vert_3;
    get_pane_vertices(hit.index, pane_vert_1, pane_vert_2, pane_vert_3);

    auto pane_vec_1 = glm::normalize(glm::vec2(pane_vert_2 - pane_vert_1));
    auto pane_vec_2 = glm::normalize(glm::vec2(pane_vert_3 - pane_vert_1));

    if (r == 'U' || r == 'u' || r == 'D' || r == 'd')
    {
        hit.dir_vec = pane_vec_1;
        hit.dir = DIR1;
    }
    else if (r == 'L' || r == 'l' || r == 'R' || r == 'r' || r == 'F' || r == 'f' || r == 'B' || r == 'b')
    {
        hit.dir_vec = pane_vec_2;
        hit.dir = DIR2;
    }
    else
    {
        throw std::runtime_error("incorrect r");
    }
}

void Rotater::get_pane_vertices(int index, glm::vec3 & v1, glm::vec3 & v2, glm::vec3 & v3) const
{
    switch (index)
    {
        case 0:
            v1 = std::move(get_vert_by_coord(0, 0, 0));
            v2 = std::move(get_vert_by_coord(2, 0, 0));
            v3 = std::move(get_vert_by_coord(0, 2, 0));
            break;
        case 1:
            v1 = std::move(get_vert_by_coord(0, 0, 2));
            v2 = std::move(get_vert_by_coord(2, 0, 2));
            v3 = std::move(get_vert_by_coord(0, 2, 2));
            break;
        case 2:
            v1 = std::move(get_vert_by_coord(0, 0, 2));
            v2 = std::move(get_vert_by_coord(0, 0, 0));
            v3 = std::move(get_vert_by_coord(0, 2, 2));
            break;
        case 3:
            v1 = std::move(get_vert_by_coord(2, 0, 2));
            v2 = std::move(get_vert_by_coord(2, 0, 0));
            v3 = std::move(get_vert_by_coord(2, 2, 2));
            break;
        case 4:
            v1 = std::move(get_vert_by_coord(0, 0, 2));
            v2 = std::move(get_vert_by_coord(2, 0, 2));
            v3 = std::move(get_vert_by_coord(0, 0, 0));
            break;
        case 5:
            v1 = std::move(get_vert_by_coord(0, 2, 2));
            v2 = std::move(get_vert_by_coord(2, 2, 2));
            v3 = std::move(get_vert_by_coord(0, 2, 0));
            break;
    }
}

glm::vec3 Rotater::get_vert_by_coord(int i, int j, int k) const
{
    return glm::vec3(cubeModel->view * cubeModel->cubelets[i][j][k].rotation * cubeModel->cubelets[i][j][k].full_translation *
                     glm::vec4(0.0f, 0.0f, 0.0f, 1.0f));
}

int Rotater::get_rot_index(int index, RotationDir dir) const
{
    switch (index)
    {
        case 0:
            if (dir == DIR1)
                return 5;
            else
                return 3;
        case 1:
            if (dir == DIR1)
                return 4;
            else
                return 2;
        case 2:
            if (dir == DIR1)
                return 5;
            else
                return 1;
        case 3:
            if (dir == DIR1)
                return 4;
            else
                return 0;
        case 4:
            if (dir == DIR1)
                return 1;
            else
                return 3;
        case 5:
            if (dir == DIR1)
                return 0;
            else
                return 2;
        default:
            throw std::runtime_error("Incorrect index");
    }
}

glm::vec3 Rotater::get_index_normal(int index) const
{
    glm::vec3 v1, v2, v3;
    get_pane_vertices(index, v1, v2, v3);

    auto e1 = v2 - v1;
    auto e2 = v3 - v1;

    return glm::normalize(glm::cross(e1, e2));
}

glm::vec3 Rotater::get_rot_vec(int index, RotationDir dir) const
{
    int rot_index = get_rot_index(index, dir);
    auto normal = get_index_normal(rot_index);
    return glm::vec3(glm::inverse(cubeModel->rotation_view) * glm::vec4(normal, 1.0f));
}

void Rotater::generate_temp_rotations(float start_angle, float end_angle, float start_speed,
                                      float acceleration, const glm::vec3 & vec)
{
    auto diff = end_angle - start_angle;

    if (std::abs(diff) < std::abs(start_speed))
        return;

    auto angle = start_angle;
    auto speed = start_speed;

    // std::cerr << "[New temp]: generate ; vec: x = " << vec.x << " ; y = " << vec.y <<
        // " ; z = " << vec.z << '\n';

    while (std::abs(end_angle - angle) > std::abs(speed))
    {
        cubeModel->rotationQueue.push(new TempRotationHeader(vec, speed));
        angle += speed;
        speed += acceleration;
    }
}

bool ScriptRotater::is_clockwise(char r) const
{
    bool clockwise = (r < 'Z');

    switch (r)
    {
        case 'U':
        case 'u':
        case 'R':
        case 'r':
        case 'F':
        case 'f':
            clockwise = !clockwise;
            break;
    }

    return clockwise;
}

void ScriptRotater::rotate_script(char r)
{
    auto clockwise = is_clockwise(r);

    auto start_angle = 0.0f;
    auto end_angle = glm::radians(clockwise ? 90.0f : -90.0f);

    auto turns = clockwise ? 1 : 3;
    turns = (r == 'D' || r == 'd' || r == 'U' || r == 'u') ? 4 - turns : turns;

    auto start_speed = glm::radians(clockwise ? 1.0f : -1.0f);
    auto acceleration = glm::radians(clockwise ? 1.0f : -1.0f);

    auto hit = get_hit_header(r);
    auto vec = get_rot_vec(hit.index, hit.dir);
    cubeModel->rotationQueue.push(new SetHitRotationHeader(hit));
    generate_temp_rotations(start_angle, end_angle, start_speed, acceleration, vec);
    cubeModel->rotationQueue.push(new PermRotationHeader(hit, turns));
}

void ScriptRotater::rotate_all_script(RotationAxis axis, bool clockwise)
{
    glm::vec3 rotationVector;
    float angle = glm::radians(90.0f);

    if (clockwise)
        angle = -angle;

    switch (axis)
    {
        case X:
            rotationVector = { 1.0f, 0.0f, 0.0f };
            break;
        case Y:
            rotationVector = { 0.0f, 1.0f, 0.0f };
            break;
        case Z:
            rotationVector = { 0.0f, 0.0f, 1.0f };
            break;
        default:
            assert(0);
    }

    float rotations = 60.0f;
    float delta = angle / rotations;
    float epsilon = 1e-5;

    for (auto a = 0.0f; std::abs(a - angle) > epsilon; a += delta)
        cubeModel->rotationQueue.push(new WholeRotationHeader(rotationVector, delta));
}

void InteractiveRotater::rotate_all_interactive(const glm::vec2 &mouse_diff)
{
    auto normal_mouse_pos = glm::normalize(glm::vec3(glm::inverse(cubeModel->view) *
                                                     glm::vec4(0.0f, 0.0f, -1.0f, 1.0f)));
    auto normal_last_mouse_pos = glm::normalize(glm::vec3(glm::inverse(cubeModel->view) *
                                                          glm::vec4(mouse_diff, -1.0f, 1.0f)));

    auto camera_rot_vec = glm::normalize(glm::cross(normal_mouse_pos, normal_last_mouse_pos));
    auto angle = std::acos(glm::dot(normal_mouse_pos, normal_last_mouse_pos));
    angle += glm::length(mouse_diff);

    cubeModel->rotationQueue.push(new WholeRotationHeader(camera_rot_vec, angle));
}

void InteractiveRotater::rotate_interactive(const glm::vec2& mouse_diff)
{
    auto hit = cubeModel->hitModel.get_header();

    auto dot = 2.0f * glm::dot(hit.dir_vec, mouse_diff);
    auto rotation_vec = get_rot_vec(hit.index, hit.dir);
    auto rot_index = get_rot_index(hit.index, hit.dir);

    bool needs_fixing;

    switch (hit.index)
    {
        case 0:
            needs_fixing = (hit.dir == DIR1);
            break;
        case 1:
            needs_fixing = (hit.dir == DIR2);
            break;
        case 2:
            needs_fixing = true;
            break;
        case 3:
            needs_fixing = false;
            break;
        case 4:
            needs_fixing = false;
            break;
        case 5:
            needs_fixing = true;
            break;
    }

    if (needs_fixing)
        dot = -dot;

    // std::cerr << "[New temp]: rotate_interactive ; vec: x = " << rotation_vec.x <<
        // " ; y = " << rotation_vec.y << " ; z = " << rotation_vec.z << '\n';

    cubeModel->rotationQueue.push(new TempRotationHeader(rotation_vec, dot));
}

void InteractiveRotater::set_interactive_dir(const glm::vec2& mouse_diff)
{
    glm::vec3 pane_vert_1, pane_vert_2, pane_vert_3;
    get_pane_vertices(cubeModel->hitModel.get_header().index, pane_vert_1, pane_vert_2, pane_vert_3);

    auto pane_vec_1 = glm::normalize(glm::vec2(pane_vert_2 - pane_vert_1));
    auto pane_vec_2 = glm::normalize(glm::vec2(pane_vert_3 - pane_vert_1));

    auto dot_1 = std::abs(glm::dot(pane_vec_1, mouse_diff));
    auto dot_2 = std::abs(glm::dot(pane_vec_2, mouse_diff));

    HitHeader hit;

    if (std::isnan(dot_1))
        throw std::runtime_error("dot is nan while getting dir");
    else
        if (dot_1 > dot_2)
        {
            hit.dir_vec = pane_vec_1;
            hit.dir = DIR1;
        }
        else
        {
            hit.dir_vec = pane_vec_2;
            hit.dir = DIR2;
        }
    
    cubeModel->rotationQueue.push(new SetHitDirRotationHeader(hit));
}

void InteractiveRotater::finish_interactive_rotation()
{
    auto angle = cubeModel->rotationQueue.get_angle();
    auto hit = cubeModel->hitModel.get_header();
    auto last_angle = cubeModel->rotationQueue.get_last_rotation().angle;
    auto last_vec = cubeModel->rotationQueue.get_last_rotation().vec;

    auto deg_angle = glm::degrees(angle);
    auto rot_index = get_rot_index(hit.index, hit.dir);

    auto turns = static_cast<int>(std::round(deg_angle / 90.0f));

    // std::cerr << glm::degrees(last_angle) << '\n';

    if (turns == 0 && std::abs(glm::degrees(last_angle)) > 2.0f)
        turns = angle > 0 ? 1 : -1;

    auto final_angle = glm::radians(90.0f * turns);
    auto rotation_dir = final_angle - angle;

    turns = (turns > 0 ? 1 : -1) * (std::abs(turns) % 4);

    if (rot_index == 4 || rot_index == 5)
        turns = -turns;

    turns = (turns + 4) % 4;
    auto start_speed = std::copysign(glm::radians(1.0f) > std::abs(last_angle) ?
                                      glm::radians(1.0f) : last_angle, rotation_dir);
    auto acceleration = std::copysign(glm::radians(0.8f), rotation_dir);

    generate_temp_rotations(angle, final_angle, start_speed, acceleration, last_vec);
    cubeModel->rotationQueue.push(new PermRotationHeader(hit, turns));
    cubeModel->rotationQueue.push(new ResetRotationHeader());
}

void InteractiveRotater::finish_interactive_rotation_to_zero()
{
    auto angle = cubeModel->rotationQueue.get_angle();
    auto hit = cubeModel->hitModel.get_header();
    auto last_angle = cubeModel->rotationQueue.get_last_rotation().angle;

    auto deg_angle = glm::degrees(angle);
    auto rot_index = get_rot_index(hit.index, hit.dir);

    // std::cerr << glm::degrees(last_angle) << '\n';

    auto final_angle = 0.0f;
    auto rotation_dir = final_angle - angle;

    auto start_speed = std::copysign(glm::radians(1.0f) > std::abs(last_angle) ?
                                     glm::radians(1.0f) : last_angle, rotation_dir);
    auto acceleration = std::copysign(glm::radians(0.8f), rotation_dir);

    generate_temp_rotations(angle, final_angle, start_speed, acceleration, get_rot_vec(hit.index, hit.dir));
    cubeModel->rotationQueue.push(new PermRotationHeader(hit, 0));
    cubeModel->rotationQueue.push(new ResetRotationHeader());
}

bool InteractiveRotater::is_going_to_rotate_finish_interactive()
{
    auto angle = cubeModel->rotationQueue.get_angle();
    auto deg_angle = glm::degrees(angle);
    auto last_angle = cubeModel->rotationQueue.get_last_rotation().angle;

    auto turns = static_cast<int>(std::round(deg_angle / 90.0f));

    if (turns == 0 && std::abs(glm::degrees(last_angle)) > 2.0f)
        turns = angle > 0 ? 1 : -1;

    return (turns != 0);
}

void InteractiveRotater::set_interactive_pos(const glm::vec2& mouse_pos)
{
    HitHeader hit_header;

    if (mouse_hitter->get_hit_position(mouse_pos, hit_header))
        cubeModel->rotationQueue.push(new SetHitPosRotationHeader(hit_header));
}

void InteractiveRotater::finish_interactive_rotation_snap(float angle, HitHeader hit,
                                                          TempRotationHeader lastRotation)
{
    auto last_angle = lastRotation.angle;
    auto last_vec = lastRotation.vec;

    auto deg_angle = glm::degrees(angle);
    auto rot_index = get_rot_index(hit.index, hit.dir);

    auto turns = static_cast<int>(std::round(deg_angle / 90.0f));

    // std::cerr << glm::degrees(last_angle) << '\n';

    if (turns == 0 && std::abs(glm::degrees(last_angle)) > 2.0f)
        turns = angle > 0 ? 1 : -1;

    auto final_angle = glm::radians(90.0f * turns);
    auto rotation_dir = final_angle - angle;

    turns = (turns > 0 ? 1 : -1) * (std::abs(turns) % 4);

    if (rot_index == 4 || rot_index == 5)
        turns = -turns;

    turns = (turns + 4) % 4;
    auto start_speed = std::copysign(glm::radians(1.0f) > std::abs(last_angle) ?
                                      glm::radians(1.0f) : last_angle, rotation_dir);
    auto acceleration = std::copysign(glm::radians(0.8f), rotation_dir);

    generate_temp_rotations(angle, final_angle, start_speed, acceleration, last_vec);
    cubeModel->rotationQueue.push(new PermRotationHeader(hit, turns));
    cubeModel->rotationQueue.push(new ResetRotationHeader());
}

bool CubeMouseHitter::get_hit_position(const glm::vec2& mouse_pos, HitHeader& hit) const
{
    auto world_ray = get_eye_ray(mouse_pos);
    auto mouse_origin = glm::vec3(0.0f, 0.0f, 0.0f);
    return hit_side(mouse_origin, world_ray, hit);
}

glm::vec3 CubeMouseHitter::get_eye_ray(const glm::vec2& mouse_pos) const
{
    auto eye_ray = glm::inverse(cubeModel->windowModel->perspectiveProjection) * glm::vec4(mouse_pos, -1.0f, 1.0f);
    eye_ray.z = -1.0f;
    eye_ray.w = 0.0f;
    return glm::normalize(glm::vec3(glm::inverse(cubeModel->translation_view) * eye_ray));
}

float CubeMouseHitter::dist_ray_to_triangle(const glm::vec3 & origin,
                                            const glm::vec3 & dir,
                                            const glm::vec3 & v0,
                                            const glm::vec3 & v1,
                                            const glm::vec3 & v2) const
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

bool CubeMouseHitter::hit_side(const glm::vec3 &mouse_origin, const glm::vec3 &world_ray,
                               HitHeader& hit) const
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

                auto& cubeData = CubeModelOpenGLData::instance();

                for (int v = 0; v < cubeData.vertices.size(); v += 3)
                {
                    auto vertex1 = transform_matrix *
                                   glm::vec4(cubeData.vertices[v], 1.0f);
                    auto vertex2 = transform_matrix *
                                   glm::vec4(cubeData.vertices[v + 1], 1.0f);
                    auto vertex3 = transform_matrix *
                                   glm::vec4(cubeData.vertices[v + 2], 1.0f);

                    float dist = dist_ray_to_triangle(mouse_origin, world_ray,
                                                      vertex1, vertex2, vertex3);

                    bool non_zero = dist > epsilon;
                    int index = cubeData.raw_vertices[v * 4 + 3];

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
                    is_anything_hit = is_hit;
                }
            }

    return is_anything_hit;
}
