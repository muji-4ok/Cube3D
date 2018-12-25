#include "CubeModel.h"


CubeModel::CubeModel()
{
    set_solved();
    reset_models_rotations();
    reset_models_translations();
}

CubeModel::~CubeModel()
{
}

void CubeModel::generate(int width, int height)
{
    Shader vertex(GL_VERTEX_SHADER, vertex_path);
    Shader fragment(GL_FRAGMENT_SHADER, fragment_path);
    shdProgram = std::move(ShaderProgram(vertex, fragment));
    set_dimensions(width, height);

    cubeVBO.generate();
    cubeVAO.generate();

    cubeVBO.bind();
    cubeVBO.setStaticData(raw_vertices);

    cubeVAO.bind();
    cubeVAO.setAttribPointer(0, 4, false, 4, 0);
    cubeVAO.enableAttribute(0);

    cubeVAO.bind();
    cubeVBO.bind();
    VAO::unbind();

    shdProgram.use();

    translation_view = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -3.0f));
    view = translation_view;
    rotation_view = glm::mat4(1.0f);
    // mat = glm::rotate(mat, glm::radians(25.0f), glm::vec3(1.0f, 0.0f, 0.0f));
    // mat = glm::rotate(mat, glm::radians(45.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    projection = glm::perspective(glm::radians(45.0f), static_cast<float>(width) / height, 0.1f, 100.0f);

    shdProgram.setUniformMatrix4fv("view", view);
    shdProgram.setUniformMatrix4fv("projection", projection);
}

void CubeModel::set_dimensions(int width, int height)
{
    this->width = width;
    this->height = height;
    projection = glm::perspective(glm::radians(45.0f), static_cast<float>(width) / height, 0.1f, 100.0f);
    shdProgram.use();
    shdProgram.setUniformMatrix4fv("projection", projection);
}

void CubeModel::set_solved()
{
    std::vector<float> y{ 1.0f, 1.0f, 0.0f };
    std::vector<float> w{ 1.0f, 1.0f, 1.0f };
    std::vector<float> r{ 1.0f, 0.0f, 0.0f };
    std::vector<float> o{ 1.0f, 0.5f, 0.0f };
    std::vector<float> b{ 0.0f, 0.0f, 1.0f };
    std::vector<float> g{ 0.0f, 1.0f, 0.0f };
    std::vector<float> n{ 0.0f, 0.0f, 0.0f };

    for (int i = 0; i < 3; ++i)
        for (int j = 0; j < 3; ++j)
            for (int k = 0; k < 3; ++k)
            {
                colors[i][j][k].clear();
                append(colors[i][j][k], k == 0 ? y : n);
                append(colors[i][j][k], k == 2 ? w : n);
                append(colors[i][j][k], i == 0 ? r : n);
                append(colors[i][j][k], i == 2 ? o : n);
                append(colors[i][j][k], j == 0 ? b : n);
                append(colors[i][j][k], j == 2 ? g : n);
            }
}

void CubeModel::draw()
{
    cubeVAO.bind();
    shdProgram.use();

    shdProgram.setUniformMatrix4fv("view", view);
    shdProgram.setUniformMatrix4fv("projection", projection);

    for (int i = 0; i < 3; ++i)
        for (int j = 0; j < 3; ++j)
            for (int k = 0; k < 3; ++k)
            {
                shdProgram.setUniform1i("i", i);
                shdProgram.setUniform1i("j", j);
                shdProgram.setUniform1i("k", k);

                auto model = models_rotate[i][j][k] * models_translate[i][j][k];

                shdProgram.setUniformMatrix4fv("model", model);
                shdProgram.setUniformVector3fv("colors", colors[i][j][k]);

                glDrawArrays(GL_TRIANGLES, 0, 36);
            }
}

void CubeModel::rotate_all_with_mouse(const glm::vec2 &diff)
{
    auto normal_mouse_pos = glm::normalize(glm::vec3(glm::inverse(view) * glm::vec4(0.0f, 0.0f, -1.0f, 1.0f)));
    auto normal_last_mouse_pos = glm::normalize(glm::vec3(glm::inverse(view) * glm::vec4(diff, -1.0f, 1.0f)));

    auto camera_rot_vec = glm::normalize(glm::cross(normal_mouse_pos, normal_last_mouse_pos));
    auto angle = std::acos(glm::dot(normal_mouse_pos, normal_last_mouse_pos));
    angle += glm::length(diff);

    constexpr float epsilon = 1e-3;

    if (std::abs(angle) > epsilon)
    {
        // std::cout << "normal_mouse: x = " << normal_mouse_pos.x << " ; y = " << normal_mouse_pos.y << " ; z = " << normal_mouse_pos.z << '\n';
        // std::cout << "normal_last_mouse: x = " << normal_last_right_mouse_pos.x << " ; y = " << normal_last_right_mouse_pos.y << " ; z = " << normal_last_right_mouse_pos.z << '\n';
        // std::cout << "rot_vec: x = " << camera_rot_vec.x << " ; y = " << camera_rot_vec.y << " ; z = " << camera_rot_vec.z << '\n';
        auto immediate_rotation = glm::rotate(glm::mat4(1.0f), angle, camera_rot_vec);
        rotation_view *= immediate_rotation;
        view *= immediate_rotation;
        // mat *= cur_mat;
    }
}

void CubeModel::rotate_facelet(const HitHeader &hit, const TempRotationHeader* rot)
{
    for (int i = 0; i < 3; ++i)
        for (int j = 0; j < 3; ++j)
            for (int k = 0; k < 3; ++k)
                if (needs_rotation(hit, i, j, k))
                    models_rotate[i][j][k] = glm::rotate(models_rotate[i][j][k], rot->angle, rot->vec);
}

bool CubeModel::get_hit_header(const glm::vec2 & mouse_pos, HitHeader &hit)
{
    auto world_ray = get_eye_ray(mouse_pos);
    auto mouse_origin = glm::vec3(0.0f, 0.0f, 0.0f);
  
    // std::cout << "new world_ray: " << world_ray.x << ' ' << world_ray.y << ' ' << world_ray.z << '\n';

    int index;
    int i, j, k;

    if (!hit_side(mouse_origin, world_ray, index, i, j, k))
        return false;

    hit.index = index;
    hit.i = i;
    hit.j = j;
    hit.k = k;
    hit.dir = DIR1;

    return true;
}

void CubeModel::get_dir(const HitHeader &hit, const glm::vec2 &diff, glm::vec2 &dir_vec,
                        Rotation_Dir &dir)
{
    glm::vec3 pane_vert_1, pane_vert_2, pane_vert_3;
    get_pane_vertices(hit.index, pane_vert_1, pane_vert_2, pane_vert_3);

    auto pane_vec_1 = glm::normalize(glm::vec2(pane_vert_2 - pane_vert_1));
    auto pane_vec_2 = glm::normalize(glm::vec2(pane_vert_3 - pane_vert_1));

    auto dot_1 = std::abs(glm::dot(pane_vec_1, diff));
    auto dot_2 = std::abs(glm::dot(pane_vec_2, diff));

    if (std::isnan(dot_1))
        throw std::runtime_error("dot is nan while getting dir");
    else
        if (dot_1 > dot_2)
        {
            dir_vec = pane_vec_1;
            dir = DIR1;
        }
        else
        {
            dir_vec = pane_vec_2;
            dir = DIR2;
        }
}

void CubeModel::get_dir_by_notation(const HitHeader & hit, char r, glm::vec2 & dir_vec, Rotation_Dir & dir)
{
    glm::vec3 pane_vert_1, pane_vert_2, pane_vert_3;
    get_pane_vertices(hit.index, pane_vert_1, pane_vert_2, pane_vert_3);

    auto pane_vec_1 = glm::normalize(glm::vec2(pane_vert_2 - pane_vert_1));
    auto pane_vec_2 = glm::normalize(glm::vec2(pane_vert_3 - pane_vert_1));

    // dir_vec = pane_vec_2;
    // dir = DIR2;
    // return;

    if (r == 'U' || r == 'u' || r == 'D' || r == 'd')
    {
        dir_vec = pane_vec_1;
        dir = DIR1;
    }
    else if (r == 'L' || r == 'l' || r == 'R' || r == 'r' || r == 'F' || r == 'f' || r == 'B', r == 'b')
    {
        dir_vec = pane_vec_2;
        dir = DIR2;
    }
}

std::unique_ptr<TempRotationHeader> CubeModel::get_rotation_header(const glm::vec2 & diff,
                                                                   const HitHeader & hit)
{
    auto dot = 2.0f * glm::dot(hit.dir_vec, diff);
    auto rotation_vec = get_rot_vec(hit);
    auto rot_index = get_rot_index(hit);

    if (needs_fixing(hit.index, rot_index))
        dot = -dot;

    return std::make_unique<TempRotationHeader>(rotation_vec, dot);
}

void CubeModel::rotate_permanent(const PermRotationHeader *perm)
{
    auto cubelets = get_rotating_cubelets(perm->hit);
    auto rotation_dir = get_cubelet_rotation(perm->hit);

    for (int i = 0; i < perm->turns; ++i)
    {
        for (auto &p : cubelets)
        {
            auto &c = *p;
            rotate_cubelet(c, rotation_dir);
        }

        for (int t = 0; t < 2; ++t)
            for (int i = 1; i < 8; ++i)
                std::swap(*cubelets[i - 1], *cubelets[i]);
    }
}

void CubeModel::rotate_cubelet(std::vector<float>& cubelet, CubeletRotation dir)
{
    switch (dir)
    {
        case X:
            swap_3(cubelet, 1, 5);
            swap_3(cubelet, 5, 0);
            swap_3(cubelet, 0, 4);
            break;
        case Y:
            swap_3(cubelet, 1, 3);
            swap_3(cubelet, 3, 0);
            swap_3(cubelet, 0, 2);
            break;
        case Z:
            swap_3(cubelet, 2, 5);
            swap_3(cubelet, 5, 3);
            swap_3(cubelet, 3, 4);
            break;
    }
}

std::vector<std::unique_ptr<RotationHeader>> CubeModel::generate_rotations(float start_angle, float end_angle,
                                                                           float start_speed,
                                                                           float acceleration,
                                                                           const glm::vec3 & vec)
{
    std::vector<std::unique_ptr<RotationHeader>> out;
    auto diff = end_angle - start_angle;

    if (std::abs(diff) < std::abs(start_speed))
        return out;

    auto angle = start_angle;
    auto speed = start_speed;

    // if (std::abs(final_angle - accum_rot_angle) < increment)
    // {
        // rotating = false;
        // auto_rotating = false;
        // accum_rot_angle = 0.0f;
        // rotate(hit_index, dir, turns, hit_i, hit_j, hit_k);
    // }
    // else
    // {
        // rot_angle = std::copysign(increment, auto_rotation_dir);
        // accum_rot_angle += rot_angle;
        // increment += glm::radians(0.8f);
    // }

    while (std::abs(end_angle - angle) > std::abs(speed))
    {
        // std::cout << "speed: " << speed << '\n';
        out.push_back(std::make_unique<TempRotationHeader>(vec, speed));
        angle += speed;
        speed += acceleration;
    }

    // while ((diff > 0 && angle < end_angle) || (diff < 0 && angle > end_angle))
    // {
        // out.push_back(std::make_unique<TempRotationHeader>(vec, speed));
        // angle += speed;
        // speed += acceleration;
    // }

    // std::cout << "generate angle: " << angle << '\n';

    // out.push_back(std::make_unique<TempRotationHeader>(vec, end_angle - angle));

    return out;
}

std::string CubeModel::to_string() const
{
    // URFDLB
    // 531420
    auto vec_to_tuple = [this](int i, int j, int k, int index) -> std::tuple<double, double, double> {
        auto first = this->colors[i][j][k][index * 3];
        auto second = this->colors[i][j][k][index * 3 + 1];
        auto third = this->colors[i][j][k][index * 3 + 2];

        return { first, second, third };
    };

    auto u = vec_to_tuple(1, 2, 1, 5);
    auto r = vec_to_tuple(2, 1, 1, 3);
    auto f = vec_to_tuple(1, 1, 2, 1);
    auto d = vec_to_tuple(1, 0, 1, 4);
    auto l = vec_to_tuple(0, 1, 1, 2);
    auto b = vec_to_tuple(1, 1, 0, 0);

    auto vec_to_char = [&vec_to_tuple, &u, &r, &f, &d, &l, &b, this](int i, int j, int k, int index) -> char {
        auto t = vec_to_tuple(i, j, k, index);

        if (t == r)
            return 'R';
        else if (t == b)
            return 'B';
        else if (t == d)
            return 'D';
        else if (t == l)
            return 'L';
        else if (t == f)
            return 'F';
        else if (t == u)
            return 'U';
    };
    std::vector<std::string> faces(6);

    for (int k = 0; k < 3; ++k)
        for (int i = 0; i < 3; ++i)
            faces[0].push_back(vec_to_char(i, 2, k, 5));

    for (int j = 2; j >= 0; --j)
        for (int k = 2; k >= 0; --k)
            faces[1].push_back(vec_to_char(2, j, k, 3));

    for (int j = 2; j >= 0; --j)
        for (int i = 0; i < 3; ++i)
            faces[2].push_back(vec_to_char(i, j, 2, 1));

    for (int k = 2; k >= 0; --k)
        for (int i = 0; i < 3; ++i)
            faces[3].push_back(vec_to_char(i, 0, k, 4));

    for (int j = 2; j >= 0; --j)
        for (int k = 0; k < 3; ++k)
            faces[4].push_back(vec_to_char(0, j, k, 2));

    for (int j = 2; j >= 0; --j)
        for (int i = 2; i >= 0; --i)
            faces[5].push_back(vec_to_char(i, j, 0, 0));

    std::cout << "faces:\n";

    for (const auto &f : faces)
        std::cout << f << '\n';

    std::string out;

    for (const auto &f : faces)
        out += f;

    std::cout << "out: " << out << '\n';

    return out;
}

bool CubeModel::needs_fixing(int index, int rot_index)
{
    switch (index)
    {
        case 0:
            return rot_index == 4 || rot_index == 5;
        case 1:
            return rot_index == 2 || rot_index == 3;
        case 2:
            return true;
        case 3:
            return false;
        case 4:
            return false;
        case 5:
            return true;
    }
}

std::array<std::vector<float>*, 8> CubeModel::get_rotating_cubelets(const HitHeader & hit)
{
    std::array<std::vector<float>*, 8> out;
    using Coord = std::tuple<int, int, int>;
    std::array<Coord, 8> coords;

    auto count_centers = [](int i, int j, int k) -> int {
        return static_cast<int>(i == 1) + static_cast<int>(j == 1) + static_cast<int>(k == 1);
    };

    bool is_center = needs_rotation(hit, 1, 1, 1);
    int c = 0;

    for (int i = 0; i < 3; ++i)
        for (int j = 0; j < 3; ++j)
            for (int k = 0; k < 3; ++k)
                if (needs_rotation(hit, i, j, k))
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

    int rot_index = get_rot_index(hit);

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

    for (int i = 0; i < 8; ++i)
        out[i] = &colors[std::get<0>(coords[i])][std::get<1>(coords[i])][std::get<2>(coords[i])];

    return out;
}

CubeletRotation CubeModel::get_cubelet_rotation(const HitHeader & hit)
{
    auto rot_index = get_rot_index(hit);

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
    }
}

glm::vec3 CubeModel::get_rot_vec(const HitHeader &hit)
{
    int rot_index = get_rot_index(hit);

    // std::cout << "rot_index: " << rot_index << '\n';

    glm::vec3 v1, v2, v3;
    get_pane_vertices(rot_index, v1, v2, v3);

    auto e1 = v2 - v1;
    auto e2 = v3 - v1;

    return glm::normalize(glm::vec3(glm::inverse(rotation_view) * glm::vec4(glm::cross(e1, e2), 1.0f)));
}

int CubeModel::get_rot_index(const HitHeader &hit)
{
    switch (hit.index)
    {
        case 0:
            if (hit.dir == DIR1)
                return 4;
            else
                return 2;
        case 1:
            if (hit.dir == DIR1)
                return 4;
            else
                return 2;
        case 2:
            if (hit.dir == DIR1)
                return 4;
            else
                return 0;
        case 3:
            if (hit.dir == DIR1)
                return 4;
            else
                return 0;
        case 4:
            if (hit.dir == DIR1)
                return 0;
            else
                return 2;
        case 5:
            if (hit.dir == DIR1)
                return 0;
            else
                return 2;
    }
}

void CubeModel::get_pane_vertices(int index, glm::vec3 & v1, glm::vec3 & v2, glm::vec3 & v3)
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

glm::vec3 CubeModel::get_vert_by_coord(int i, int j, int k)
{
    return glm::vec3(view * models_rotate[i][j][k] * models_translate_full[i][j][k] *
                     glm::vec4(0.0f, 0.0f, 0.0f, 1.0f));
}

glm::vec3 CubeModel::get_eye_ray(const glm::vec2& mouse_pos)
{
    auto eye_ray = glm::inverse(projection) * glm::vec4(mouse_pos, -1.0f, 1.0f);
    eye_ray.z = -1.0f;
    eye_ray.w = 0.0f;

    return glm::normalize(glm::vec3(glm::inverse(translation_view) * eye_ray));
}

bool CubeModel::hit_side(const glm::vec3 &mouse_origin, const glm::vec3 &world_ray,
                         int &index, int &hit_i, int &hit_j, int &hit_k)
{
    bool is_anything_hit = false;
    float min_hit_dist = std::numeric_limits<float>::max();

    for (int i = 0; i < 3; ++i)
        for (int j = 0; j < 3; ++j)
            for (int k = 0; k < 3; ++k)
            {
                auto transform_matrix = view * models_rotate[i][j][k] * models_translate_full[i][j][k];

                constexpr float epsilon = 1e-8;

                float min_dist = std::numeric_limits<float>::max();
                int min_index;
                bool is_hit = false;

                for (int v = 0; v < vertices.size(); v += 3)
                {
                    auto vertex1 = transform_matrix * glm::vec4(vertices[v], 1.0f);
                    auto vertex2 = transform_matrix * glm::vec4(vertices[v + 1], 1.0f);
                    auto vertex3 = transform_matrix * glm::vec4(vertices[v + 2], 1.0f);

                    float dist = dist_ray_to_triangle(mouse_origin, world_ray, vertex1, vertex2, vertex3);

                    // std::cout << dist << '\n';

                    bool non_zero = dist > epsilon;
                    int index = raw_vertices[v * 4 + 3];

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
                    hit_i = i;
                    hit_j = j;
                    hit_k = k;
                    index = min_index;
                    is_anything_hit = is_hit;
                }
            }

    return is_anything_hit;
}

float CubeModel::dist_ray_to_plane(const glm::vec3 & ray_origin, const glm::vec3 & ray_dir,
                                   const glm::vec3 & plane_point, const glm::vec3 & plane_normal)
{
    auto ray_plane_angle = glm::dot(ray_dir, plane_normal);
    constexpr float epsilon = 1e-6;

    if (std::abs(ray_plane_angle) <= epsilon)
        return -1.0f;

    auto rp = plane_point - ray_origin;

    return glm::dot(rp, plane_normal) / ray_plane_angle;
}

float CubeModel::dist_ray_to_triangle(const glm::vec3 & origin, const glm::vec3 & dir,
                                      const glm::vec3 & v0, const glm::vec3 & v1, const glm::vec3 & v2)
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

bool CubeModel::needs_rotation(const HitHeader &hit, int i, int j, int k)
{
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
    }
}

void CubeModel::reset_models_rotations()
{
    for (int i = 0; i < 3; ++i)
        for (int j = 0; j < 3; ++j)
            for (int k = 0; k < 3; ++k)
                models_rotate[i][j][k] = glm::mat4(1.0f);
}

HitHeader CubeModel::notation_to_hit_header(char r)
{
    HitHeader hit;
    Rotation_Dir dir;
    glm::vec2 dir_vec;

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

    get_dir_by_notation(hit, r, dir_vec, dir);
    hit.dir = dir;
    hit.dir_vec = dir_vec;

    return hit;
}

std::string CubeModel::gen_solution() const
{
    auto cube_string = to_string();
    char* c_cube = const_cast<char *>(cube_string.c_str());
    std::string sol;

    while (true)
        try
        {
            sol = std::string(solution(
                c_cube,
                42,
                1000,
                0,
                "cache"
            ));

            break;
        }
        catch (const std::exception &e)
        {
            std::cout << "error\n";
            std::cout << e.what() << '\n';
        }

    std::cout << sol << '\n';

    std::istringstream iss(sol);
    std::vector<std::string> moves;
    std::copy(std::istream_iterator<std::string>(iss),
              std::istream_iterator<std::string>(),
              std::back_inserter(moves));
    std::string seq;
    auto to_notation = [](const std::string &s) -> std::string {
        bool doub = false;
        bool revr = false;

        if (s.size() > 1)
        {
            doub = (s[1] == '2');
            revr = (s[1] == '\'');
        }

        std::string out;

        if (doub)
        {
            out.assign(2, s[0]);
        }
        else
        {
            auto c = s[0];

            if (revr)
                switch (c)
                {
                    case 'U':
                        c = 'u';
                        break;
                    case 'D':
                        c = 'd';
                        break;
                    case 'L':
                        c = 'l';
                        break;
                    case 'R':
                        c = 'r';
                        break;
                    case 'F':
                        c = 'f';
                        break;
                    case 'B':
                        c = 'b';
                        break;
                }

            out.push_back(c);
        }

        return out;
    };

    for (const auto &m : moves)
        seq += to_notation(m);

    return seq;
}

std::vector<std::unique_ptr<RotationHeader>> CubeModel::get_rotations_to_orthogonal(
    float angle,
    const HitHeader &hit,
    const TempRotationHeader &lastRotation
)
{
    auto deg_angle = glm::degrees(angle);
    int rot_index = get_rot_index(hit);

    auto turns = static_cast<int>(std::round(deg_angle / 90.0f));

    if (turns == 0 && std::abs(glm::degrees(lastRotation.angle)) > 2.0f)
        turns = angle > 0 ? 1 : -1;

    auto final_angle = glm::radians(90.0f * turns);
    auto rotation_dir = final_angle - angle;

    turns = (turns > 0 ? 1 : -1) * (std::abs(turns) % 4);

    if (rot_index == 4 || rot_index == 5)
        turns = -turns;

    turns = (turns + 4) % 4;
    // std::cout << "rot_angle: " << lastRotation.angle << '\n';
    auto start_speed = std::copysign(glm::radians(1.0f) > std::abs(lastRotation.angle) ?
                                     glm::radians(1.0f) : lastRotation.angle, rotation_dir);
    auto acceleration = std::copysign(glm::radians(0.8f), rotation_dir);
    auto out = generate_rotations(angle, final_angle, start_speed,
                                  acceleration, lastRotation.vec);
    out.push_back(std::make_unique<PermRotationHeader>(hit, turns));
    out.push_back(std::make_unique<ResetRotationHeader>());

    return out;
}

std::vector<std::unique_ptr<RotationHeader>> CubeModel::get_rotations_for_script(const HitHeader &hit,
                                                                                 bool clockwise, char r)
{
    auto start_angle = 0.0f;
    auto end_angle = glm::radians(clockwise ? 90.0f : -90.0f);
    auto turns = clockwise ? 1 : 3;
    turns = (r == 'D' || r == 'd' || r == 'U' || r == 'u') ? 4 - turns : turns;
    auto start_speed = glm::radians(clockwise ? 1.0f : -1.0f);
    auto acceleration = glm::radians(clockwise ? 1.0f : -1.0f);
    auto vec = get_rot_vec(hit);
    auto out = generate_rotations(start_angle, end_angle, start_speed, acceleration, vec);
    out.push_back(std::make_unique<PermRotationHeader>(hit, turns));
    // std::cout << "vec: x = " << vec.x << " ; y = " << vec.y << " ; z = " << vec.z << '\n';

    return out;
}

void CubeModel::reset_models_translations()
{
    for (int i = 0; i < 3; ++i)
        for (int j = 0; j < 3; ++j)
            for (int k = 0; k < 3; ++k)
            {
                glm::mat4 model = glm::mat4(1.0f);
                model = glm::translate(model, glm::vec3(0.35f * (i - 1), 0.35f * (j - 1), 0.35f * (k - 1)));

                auto model_full_size = glm::scale(model, glm::vec3(0.35f));
                model = glm::scale(model, glm::vec3(1.0f / 3.0f));

                models_translate[i][j][k] = std::move(model);
                models_translate_full[i][j][k] = std::move(model_full_size);
            }
}

template<typename T>
void append(std::vector<T>& v1, const std::vector<T>& v2)
{
    v1.insert(v1.end(), v2.begin(), v2.end());
}

template<typename T>
void swap_3(std::vector<T>& v, int offset1, int offset2)
{
    for (int i = 0; i < 3; ++i)
        std::swap(v[offset1 * 3 + i], v[offset2 * 3 + i]);
}
