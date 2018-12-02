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

void CubeModel::rotate_facelet(const HitHeader &hit, const RotationHeader& rot)
{
    for (int i = 0; i < 3; ++i)
        for (int j = 0; j < 3; ++j)
            for (int k = 0; k < 3; ++k)
                if (needs_rotation(hit, i, j, k))
                    models_rotate[i][j][k] = glm::rotate(models_rotate[i][j][k], rot.angle, rot.vec);
}

bool CubeModel::get_hit_header(const glm::vec2 & mouse_pos, HitHeader &hit)
{
    auto world_ray = get_eye_ray(mouse_pos);
    auto mouse_origin = glm::vec3(0.0f, 0.0f, 0.0f);

    int index;
    int i, j, k;

    if (!hit_side(mouse_origin, world_ray, index, i, j, k))
        return false;

    hit.index = index;
    hit.i = i;
    hit.j = j;
    hit.k = k;
    hit.dir = DIR1;
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

RotationHeader CubeModel::get_rotation_header(const glm::vec2 & diff, const HitHeader & hit)
{
    auto dot = 2.0f * glm::dot(hit.dir_vec, diff);
    auto rotation_vec = get_rot_vec(hit);
    auto rot_index = get_rot_index(hit);

    if (needs_fixing(hit.index, rot_index))
        dot = -dot;

    return { rotation_vec, dot };
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

glm::vec3 CubeModel::get_rot_vec(const HitHeader &hit)
{
    int rot_index = get_rot_index(hit);

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

    return glm::normalize(glm::vec3(glm::inverse(view) * eye_ray));
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
                auto transform_matrix = view * models_rotate[i][j][k] * models_translate[i][j][k];

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
