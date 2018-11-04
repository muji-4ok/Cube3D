#include "App.h"

App * CallbackCaller::app = nullptr;

void App::error_callback(int error, const char *description)
{
    std::cerr << "GLFW Error (" << error << ") message: " << description << '\n';
}

void App::key_callback(GLFWwindow *window, int key, int scancode, int action, int mods)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}

void App::mouse_callback(GLFWwindow *window, int button, int action, int mods)
{
    if (button == GLFW_MOUSE_BUTTON_LEFT)
    {
        mouse_pressed = (action == GLFW_PRESS);

        if (action != GLFW_PRESS)
        {
            this->rotating = false;
            this->rotation_angle = 0.0f;

            return;
        }

        double x_pos, y_pos;
        glfwGetCursorPos(window, &x_pos, &y_pos);

        last_mouse_pos = get_mouse_pos(x_pos, y_pos, width, height);

        // std::cout << x_pos << ' ' << y_pos << '\n';

        world_ray = get_eye_ray(x_pos, y_pos, width, height, projection, view);
        auto mouse_origin = glm::vec3(0.0f, 0.0f, 0.0f);
        // mouse_origin = glm::vec3(last_mouse_pos, 0.0f);

        // std::cout << world_ray.x << ' ' << world_ray.y << ' ' << world_ray.z << '\n';

        int index;
        int hit_i, hit_j, hit_k;

        if (!hit_side(mouse_origin, world_ray, index, hit_i, hit_j, hit_k))
            return;

        this->hit_index = index;
        this->hit_i = hit_i;
        this->hit_j = hit_j;
        this->hit_k = hit_k;

        this->has_dir = false;
        this->has_original = false;
        this->rotating = true;
        this->rotation_angle = 0.0f;
    }
}

void App::frame_buffer_change_callback(GLFWwindow *window, int width, int height)
{
    glViewport(0, 0, width, height);
    projection = glm::perspective(glm::radians(45.0f), static_cast<float>(width) / height, 0.1f, 100.0f);
    shdProgram.setUniformMatrix4fv("projection", projection);
    this->width = width;
    this->height = height;
}

float dist_from_ray_to_triangle(const glm::vec3 & origin, const glm::vec3 & dir,
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

glm::vec3 get_eye_ray(double x_pos, double y_pos, int width, int height,
                      const glm::mat4 & projection, const glm::mat4 & view)
{
    float x = 2.0 * x_pos / width - 1.0;
    float y = 2.0 * y_pos / height - 1.0;
    y = -y;

    auto eye_ray = glm::inverse(projection) * glm::vec4(x, y, -1.0f, 1.0f);
    eye_ray.z = -1.0f;
    eye_ray.w = 0.0f;

    return glm::normalize(glm::vec3(glm::inverse(view) * eye_ray));
}

glm::vec3 get_eye_ray(const glm::vec2 & mouse_pos, const glm::mat4 & projection, const glm::mat4 & view)
{
    auto eye_ray = glm::inverse(projection) * glm::vec4(mouse_pos, -1.0f, 1.0f);
    eye_ray.z = -1.0f;
    eye_ray.w = 0.0f;

    return glm::normalize(glm::vec3(glm::inverse(view) * eye_ray));
}

bool needs_rotation(int index, int i, int j, int k, int hit_i, int hit_j, int hit_k)
{
    switch (index)
    {
        case 0:
            return k == 0;
        case 1:
            return k == 2;
        case 2:
            return i == 0;
        case 3:
            return i == 2;
        case 4:
            return j == 0;
        case 5:
            return j == 2;
    }
}

glm::vec3 get_rotation_vec(int index)
{
    switch (index)
    {
        case 0:
        case 1:
            return glm::vec3(0.0f, 0.0f, 1.0f);
        case 2:
        case 3:
            return glm::vec3(1.0f, 0.0f, 0.0f);
        case 4:
        case 5:
            return glm::vec3(0.0f, 1.0f, 0.0f);
    }
}

float App::get_rotation_angle(int index, int i, int j, int k,
                              const glm::vec2 &original_mouse_pos, const glm::vec2 &mouse_pos)
{
    // glm::vec3 pane_vert_1, pane_vert_2, pane_vert_3;
    // get_pane_vertices(index, pane_vert_1, pane_vert_2, pane_vert_3);
    // auto pane_edge_2 = pane_vert_3 - pane_vert_1;

    if (!has_dir)
    {
        auto dist = glm::distance(original_mouse_pos, mouse_pos);

        constexpr float epsilon = 1e-4;

        if (dist < epsilon)
            return 0.0f;
        else
            return dist;
    }
    else if (!has_original)
    {
        has_original = true;

        glm::vec3 pane_vert_2, pane_vert_3;
        get_pane_vertices(index, pane_vert_1, pane_vert_2, pane_vert_3);
        std::cout << "v1: x = " << pane_vert_1.x << " ; y = " << pane_vert_1.y << " ; z = " << pane_vert_1.z << '\n';

        auto pane_edge_1 = pane_vert_2 - pane_vert_1;
        auto pane_edge_2 = pane_vert_3 - pane_vert_1;

        std::cout << "edge_1: x = " << pane_edge_1.x << " ; y = " << pane_edge_1.y << " ; z = " << pane_edge_1.z << '\n';
        std::cout << "edge_2: x = " << pane_edge_2.x << " ; y = " << pane_edge_2.y << " ; z = " << pane_edge_2.z << '\n';

        normal = glm::normalize(glm::cross(pane_edge_1, pane_edge_2));

        auto mouse_origin = glm::vec3(0.0f, 0.0f, 0.0f);
        auto original_ray = glm::normalize(world_ray);

        vert = get_vert_by_coord(hit_i, hit_j, hit_k);

        auto original_dist = intersect_ray_plane(normal, vert, mouse_origin, original_ray);
        
        auto original_intersect_point = original_ray * original_dist;

        auto rot_index = get_rot_index(index, dir);
        // std::cout << "rot_index: " << rot_index << ' ' << "index: " << index << '\n';
        glm::vec3 rot_pane_vert_1, rot_pane_vert_2, rot_pane_vert_3;
        get_rot_pane_vertices(rot_index, hit_i, hit_j, hit_k, rot_pane_vert_1, rot_pane_vert_2, rot_pane_vert_3);

        auto rot_pane_edge_1 = rot_pane_vert_2 - rot_pane_vert_1;
        auto rot_pane_edge_2 = rot_pane_vert_3 - rot_pane_vert_1;

        rot_normal = glm::normalize(glm::cross(rot_pane_edge_1, rot_pane_edge_2));

        pane_origin = (rot_pane_edge_1 + rot_pane_edge_2) / 2.0f;

        auto projected_original_intersect_point = project_point_onto_plane(vert, rot_normal, original_intersect_point);

        original_mouse_dir = glm::normalize(projected_original_intersect_point - pane_origin);
    }
    // static float angle = 0.0f;
    // return angle += 0.01;
    // return std::acos(glm::dot(glm::normalize(original_mouse_pos), glm::normalize(mouse_pos)));

    // glm::vec3 pane_vert_1, pane_vert_2, pane_vert_3;
    // get_pane_vertices(index, pane_vert_1, pane_vert_2, pane_vert_3);

    // std::cout << "v1: x = " << pane_vert_1.x << " ; y = " << pane_vert_1.y << " ; z = " << pane_vert_1.z << '\n';
    // std::cout << "v2: x = " << pane_vert_2.x << " ; y = " << pane_vert_2.y << " ; z = " << pane_vert_2.z << '\n';
    // std::cout << "v3: x = " << pane_vert_3.x << " ; y = " << pane_vert_3.y << " ; z = " << pane_vert_3.z << '\n';

    // auto pane_edge_1 = pane_vert_2 - pane_vert_1;
    // auto pane_edge_2 = pane_vert_3 - pane_vert_1;

    // std::cout << "edge_1: x = " << pane_edge_1.x << " ; y = " << pane_edge_1.y << " ; z = " << pane_edge_1.z << '\n';
    // std::cout << "edge_2: x = " << pane_edge_2.x << " ; y = " << pane_edge_2.y << " ; z = " << pane_edge_2.z << '\n';

    // auto normal = glm::normalize(glm::cross(pane_edge_1, pane_edge_2));

    // std::cout << "normal: x = " << normal.x << " ; y = " << normal.y << " ; z = " << normal.z << '\n';

    // auto original_ray = glm::normalize(get_eye_ray(original_mouse_pos, projection, view));
    auto ray = glm::normalize(get_eye_ray(mouse_pos, projection, view));
    auto mouse_origin = glm::vec3(0.0f, 0.0f, 0.0f);
    // mouse_origin = glm::vec3(mouse_pos, 0.0f);

    // std::cout << "orignal_ray: x = " << original_ray.x << " ; y = " << original_ray.y << " ; z = " << original_ray.z << '\n';
    // std::cout << "ray: x = " << ray.x << " ; y = " << ray.y << " ; z = " << ray.z << '\n';

    // auto original_dist = intersect_ray_plane(normal, pane_vert_1, mouse_origin, original_ray);
    // auto vert = get_vert_by_coord(hit_i, hit_j, hit_k);
    auto dist = intersect_ray_plane(normal, vert, mouse_origin, ray);

    // std::cout << "original_dist: " << original_dist << ' ' << "dist: " << dist << '\n';

    // auto original_intersect_point = original_ray * original_dist;
    auto intersect_point = ray * dist;

    auto projected_intersect_point = project_point_onto_plane(vert, rot_normal, intersect_point);

    // up = glm::normalize(glm::cross(normal, intersect_point - pane_origin));
    // center = intersect_point;
    // rotation_view = glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), intersect_point, up);

    // auto pane_origin = (pane_edge_1 + pane_edge_2) / 2.0f;

    // std::cout << "pane_origin: x = " << pane_origin.x << " ; y = " << pane_origin.y << " ; z = " << pane_origin.z << '\n';

    // auto original_mouse_dir = glm::normalize(original_intersect_point - pane_origin);
    auto mouse_dir = glm::normalize(projected_intersect_point - pane_origin);

    std::cout << "original_mouse_dir: x = " << original_mouse_dir.x << " ; y = " << original_mouse_dir.y << " ; z = " << original_mouse_dir.z << '\n';
    std::cout << "mouse_dir: x = " << mouse_dir.x << " ; y = " << mouse_dir.y << " ; z = " << mouse_dir.z << '\n';

    auto dot = glm::dot(original_mouse_dir, mouse_dir);
    // auto dot = glm::dot(original_intersect_point, intersect_point);
    // auto dot = glm::dot(original_mouse_pos, mouse_pos);

    constexpr float epsilon = 1e-5;

    if (std::abs(dot - 1.0f) <= epsilon)
        return 0.0f;

    auto cross = glm::normalize(glm::cross(original_mouse_dir, mouse_dir));

    std::cout << "rot_normal: x = " << rot_normal.x << " ; y = " << rot_normal.y << " ; z = " << rot_normal.z << '\n';
    std::cout << "cross: x = " << cross.x << " ; y = " << cross.y << " ; z = " << cross.z << '\n';

    bool similar = are_similary_oriented(cross, rot_normal);

    std::cout << similar << '\n';

    // std::cout << dot << '\n';

    auto acos = std::acos(dot);

    acos *= 2.0f;

    auto rot_index = get_rot_index(index, dir);

    if (!((rot_index == 0 || rot_index == 1) && index == 5))
        acos = -acos;

    if (!similar)
        acos = -acos;
    // acos = std::copysign(acos, -similar);

    std::cout << glm::degrees(acos) << '\n';

    // std::cout << acos << '\n';

    if (std::abs(acos) < epsilon)
        return 0.0f;
    else
        return acos; // std::copysign(acos, dot);

    // auto pane_origin = glm::vec3(view * cube.models_full_size[2][1][1] * glm::vec4(0.35f, 0.0f, 0.0f, 1.0f));
    // auto original_mouse_pos_3d = glm::vec3(original_mouse_pos, 0.0f);
    // auto mouse_pos_3d = glm::vec3(mouse_pos, 0.0f);

    // auto mouse_diff_vec = glm::normalize(mouse_pos_3d - original_mouse_pos_3d);

    // auto dot_1 = std::abs(glm::dot(pane_vec_1, mouse_diff_vec));
    // auto dot_2 = std::abs(glm::dot(pane_vec_2, mouse_diff_vec));

    // std::cout << "dot_1: " << dot_1 << ' ' << "dot_2: " << dot_2 << '\n';

    // if (!std::isnan(dot_1))
        // if (dot_2 > dot_1)
            // std::cout << "VERTICAL\n";
        // else
            // std::cout << "HORIZONTAL\n";

    // auto v1 = glm::normalize(original_mouse_pos_3d - pane_origin);
    // auto v2 = glm::normalize(mouse_pos_3d - pane_origin);

    // auto dot = glm::dot(v1, v2);
    // auto dot = glm::dot(original_mouse_pos, mouse_pos);

    // constexpr float epsilon = 1e-5;

    // if (std::abs(dot - 1.0f) <= epsilon)
        // return 0.0f;

    // std::cout << dot << '\n';

    // auto acos = 2 * std::acos(dot);

    // if (std::abs(acos) < epsilon)
        // return 0.0f;
    // else
        // return std::copysign(acos, glm::cross(original_mouse_pos_3d, mouse_pos_3d).z);
}

void App::get_pane_vertices(int index, glm::vec3 & v1, glm::vec3 & v2, glm::vec3 & v3)
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

void App::get_rot_pane_vertices(int index, int i, int j, int k, glm::vec3 & v1, glm::vec3 & v2, glm::vec3 & v3)
{
    switch (index)
    {
        case 0:
        case 1:
            v1 = std::move(get_vert_by_coord(0, 0, k));
            v2 = std::move(get_vert_by_coord(2, 0, k));
            v3 = std::move(get_vert_by_coord(0, 2, k));
            break;
        case 2:
        case 3:
            v1 = std::move(get_vert_by_coord(i, 0, 2));
            v2 = std::move(get_vert_by_coord(i, 0, 0));
            v3 = std::move(get_vert_by_coord(i, 2, 2));
            break;
        case 4:
        case 5:
            v1 = std::move(get_vert_by_coord(0, j, 2));
            v2 = std::move(get_vert_by_coord(2, j, 2));
            v3 = std::move(get_vert_by_coord(0, j, 0));
            break;
    }
}

glm::vec3 App::get_vert_by_coord(int i, int j, int k)
{
    return glm::vec3(view * cube.models_full_size[i][j][k] *
                     glm::vec4(0.35f * (i - 1), 0.35f * (j - 1), 0.35f * (k - 1), 1.0f));
}

glm::vec3 App::get_rot_vec(int index, int i, int j, int k, Rotation_Dir dir)
{
    int rot_index = get_rot_index(index, dir);

    glm::vec3 v1, v2, v3;
    get_pane_vertices(rot_index, v1, v2, v3);

    auto e1 = v2 - v1;
    auto e2 = v3 - v1;

    return glm::normalize(glm::cross(e1, e2));
}

bool App::needs_rotation(int index, Rotation_Dir dir, int hit_i, int hit_j, int hit_k, int i, int j, int k)
{
    switch (index)
    {
        case 0:
            if (dir == DIR1)
                return j == hit_j;
            else
                return i == hit_i;
        case 1:
            if (dir == DIR1)
                return j == hit_j;
            else
                return i == hit_i;
        case 2:
            if (dir == DIR1)
                return j == hit_j;
            else
                return k == hit_k;
        case 3:
            if (dir == DIR1)
                return j == hit_j;
            else
                return k == hit_k;
        case 4:
            if (dir == DIR1)
                return k == hit_k;
            else
                return i == hit_i;
        case 5:
            if (dir == DIR1)
                return k == hit_k;
            else
                return i == hit_i;
    }
}

Rotation_Dir App::get_dir(int index, const glm::vec2 &original_mouse_pos, const glm::vec2 &mouse_pos)
{
    glm::vec3 pane_vert_1, pane_vert_2, pane_vert_3;
    get_pane_vertices(index, pane_vert_1, pane_vert_2, pane_vert_3);

    auto pane_vec_1 = glm::normalize(pane_vert_2 - pane_vert_1);
    auto pane_vec_2 = glm::normalize(pane_vert_3 - pane_vert_1);

    auto pane_origin = glm::vec3(view * cube.models_full_size[2][1][1] * glm::vec4(0.35f, 0.0f, 0.0f, 1.0f));
    auto original_mouse_pos_3d = glm::vec3(original_mouse_pos, 0.0f);
    auto mouse_pos_3d = glm::vec3(mouse_pos, 0.0f);

    auto mouse_diff_vec = glm::normalize(mouse_pos_3d - original_mouse_pos_3d);

    auto dot_1 = std::abs(glm::dot(pane_vec_1, mouse_diff_vec));
    auto dot_2 = std::abs(glm::dot(pane_vec_2, mouse_diff_vec));

    // std::cout << "dot_1: " << dot_1 << ' ' << "dot_2: " << dot_2 << '\n';
    if (std::isnan(dot_1))
        return DIR1;
    else
        return dot_1 > dot_2 ? DIR1 : DIR2;
}

int App::get_rot_index(int index, Rotation_Dir dir)
{
    switch (index)
    {
        case 0:
            if (dir == DIR1)
                return 4;
            else
                return 2;
        case 1:
            if (dir == DIR1)
                return 4;
            else
                return 2;
        case 2:
            if (dir == DIR1)
                return 4;
            else
                return 0;
        case 3:
            if (dir == DIR1)
                return 4;
            else
                return 0;
        case 4:
            if (dir == DIR1)
                return 0;
            else
                return 2;
        case 5:
            if (dir == DIR1)
                return 0;
            else
                return 2;
    }
}

glm::vec3 App::project_point_onto_plane(const glm::vec3 &plane_point, const glm::vec3 &plane_normal,
                                        const glm::vec3 &point)
{
    auto v = point - plane_point;
    auto d = glm::dot(v, plane_normal);

    return point - plane_normal * d;
}

bool App::are_similary_oriented(const glm::vec3 & v1, const glm::vec3 & v2)
{
    auto cmp = [](const float &val1, const float &val2) {
        return std::abs(val1) < std::abs(val2);
    };

    auto max1 = std::max({ v1.x, v1.y, v1.z }, cmp);
    auto max2 = std::max({ v2.x, v2.y, v2.z }, cmp);

    return max1 * max2 > 0;
    // constexpr float epsilon = 1e-1;
    // bool is_x_good = std::abs(v1.x) >= epsilon && std::abs(v2.x) >= epsilon;
    // bool is_y_good = std::abs(v1.y) >= epsilon && std::abs(v2.y) >= epsilon;
    // bool is_z_good = std::abs(v1.z) >= epsilon && std::abs(v2.z) >= epsilon;
    // if (is_x_good)
        // return v1.x * v2.x > 0;
    // else if (is_y_good)
        // return v1.y * v2.y > 0;
    // else if (is_z_good)
        // return v1.z * v2.z > 0;
    // else
        // return false;
        // throw std::runtime_error("vectors not collinear");
}

glm::vec2 get_mouse_pos(double x_pos, double y_pos, int width, int height)
{
    float x = 2.0 * x_pos / width - 1.0;
    float y = 2.0 * y_pos / height - 1.0;
    y = -y;

    return glm::vec2(x, y);
}

float intersect_ray_plane(const glm::vec3 & normal, const glm::vec3 & plane_point, const glm::vec3 & ray_origin,
                          const glm::vec3 & ray_dir)
{
    float denom = glm::dot(normal, ray_dir);
    constexpr float epsilon = 1e-6;

    // std::cout << "denom: " << denom << '\n';

    if (std::abs(denom) > epsilon)
    {
        auto ray_origin_to_plane_origin = plane_point - ray_origin;

        return glm::dot(ray_origin_to_plane_origin, normal) / denom;
    }
    else
    {
        return 0.0f;
    }
}

App::App(int width, int height) : width(width), height(height)
{
    if (!glfwInit())
    {
        std::cerr << "GLFW init failed\n";
        exit(1);
    }

    CallbackCaller::app = this;

    glfwSetErrorCallback(CallbackCaller::error_callback_caller);

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    window = glfwCreateWindow(width, height, "Window", nullptr, nullptr);

    if (!window)
    {
        std::cerr << "GLFW window creation failed\n";
        exit(1);
    }

    glfwMakeContextCurrent(window);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cerr << "Failed to initialize GLAD\n";
        exit(1);
    }


    glfwSetKeyCallback(window, CallbackCaller::key_callback_caller);
    glfwSetMouseButtonCallback(window, CallbackCaller::mouse_callback_caller);
    glfwSetFramebufferSizeCallback(window, CallbackCaller::frame_buffer_change_callback_caller);

    run();
}

void App::run()
{
    prepare();

    while (!glfwWindowShouldClose(window))
    {
        calculate();
        draw();
        glfwSwapBuffers(window);
        update_fps();
        glfwPollEvents();
    }
}

void App::prepare()
{
    glClearColor(0.1f, 0.2f, 0.4f, 1.0f);
    glViewport(0, 0, width, height);
    glEnable(GL_DEPTH_TEST);

    std::string vertex_path = R"(D:\Egor\projects\cpp\Graphics_Experiments\Rubiks_Cube\shaders\standardVertex.glsl)";
    std::string fragment_path = R"(D:\Egor\projects\cpp\Graphics_Experiments\Rubiks_Cube\shaders\standardFragment.glsl)";

    vertex = std::move(Shader(GL_VERTEX_SHADER, vertex_path));
    fragment = std::move(Shader(GL_FRAGMENT_SHADER, fragment_path));
    shdProgram = std::move(ShaderProgram(vertex, fragment));

    cubeVBO.generate();
    cubeVAO.generate();

    cubeVBO.bind();
    cubeVBO.setStaticData(cube.vertices);

    cubeVAO.bind();
    cubeVAO.setAttribPointer(0, 4, false, 4, 0);
    cubeVAO.enableAttribute(0);

    cubeVAO.bind();
    cubeVBO.bind();

    shdProgram.use();
    
    view = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -3.0f));
    projection = glm::perspective(glm::radians(45.0f), static_cast<float>(width) / height, 0.1f, 100.0f);

    shdProgram.setUniformMatrix4fv("view", view);
    shdProgram.setUniformMatrix4fv("projection", projection);
}

void App::calculate()
{
    // std::cout << rotating << '\n';

    // auto rotation_vec = get_rotation_vec(hit_index);
    double x_pos, y_pos;
    glfwGetCursorPos(window, &x_pos, &y_pos);

    auto mouse_pos = get_mouse_pos(x_pos, y_pos, width, height);

    auto rot_angle = get_rotation_angle(hit_index, hit_i, hit_j, hit_k, last_mouse_pos, mouse_pos);

    if (!has_dir && rot_angle)
    {
        has_dir = true;
        dir = get_dir(hit_index, last_mouse_pos, mouse_pos);
    }

    auto rotation_vec = get_rot_vec(hit_index, hit_i, hit_j, hit_k, dir);

    // std::cout << rot_angle << '\n';

    for (int i = 0; i < 3; ++i)
        for (int j = 0; j < 3; ++j)
            for (int k = 0; k < 3; ++k)
            {
                glm::mat4 model = glm::mat4(1.0f);

                // model = glm::mat4(1.0f);
                // model = glm::rotate(model, glm::radians(45.0f), glm::vec3(1.0f, 1.0f, 0.0f));
                // model = glm::translate(model, glm::vec3(0.35f * (i - 1), 0.35f * (j - 1), 0.35f * (k - 1)));
                
                // if (true) // rotating && App::needs_rotation(hit_index, dir, hit_i, hit_j, hit_k, i, j, k))
                // {
                    // auto position = glm::vec3(0.35f * (i - 1), 0.35f * (j - 1), 0.35f * (k - 1));
                    // auto look_at = glm::lookAt(position, obj_position, glm::vec3(0.0f, 1.0f, 0.0f));
                    // model = glm::mat4(1.0f);
                    // model = model * look_at;
                    // model = glm::rotate(model, glm::radians(45.0f), glm::vec3(1.0f, 1.0f, 0.0f));
                    // model = glm::translate(model, position);
                // }
                // else
                // {
                    // model = glm::mat4(1.0f);
                    // model = glm::rotate(model, glm::radians(45.0f), glm::vec3(1.0f, 1.0f, 0.0f));
                    // model = glm::translate(model, glm::vec3(0.35f * (i - 1), 0.35f * (j - 1), 0.35f * (k - 1)));
                // }
                    // model = model * rotation_view;
                if (rotating && App::needs_rotation(hit_index, dir, hit_i, hit_j, hit_k, i, j, k))
                    model = glm::rotate(model, rot_angle, rotation_vec);
                else
                    model = glm::mat4(1.0f);

                model = glm::rotate(model, glm::radians(45.0f), glm::vec3(1.0f, 1.0f, 0.0f));
                model = glm::translate(model, glm::vec3(0.35f * (i - 1), 0.35f * (j - 1), 0.35f * (k - 1)));

                auto model_full_size = glm::scale(model, glm::vec3(0.35f));
                model = glm::scale(model, glm::vec3(1.0f / 3.0f));

                cube.models[i][j][k] = std::move(model);
                cube.models_full_size[i][j][k] = std::move(model_full_size);
            }

    if (rotating)
        rotation_angle += 1.0f;
}

void App::draw()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // static double rotation_angle = 0.0f;
    // auto model = glm::mat4(1.0f);
    // obj_position = glm::vec3(-4.0f, 5 * std::sin(rotation_angle), -4.0f);
    // model = glm::translate(model, obj_position);
    // model = glm::rotate(model, glm::radians(45.0f), glm::vec3(1.0f, 0.0f, 1.0f));
    // shdProgram.setUniformMatrix4fv("model", model);
    // rotation_angle += 0.03f;
    // glDrawArrays(GL_TRIANGLES, 0, 36);

    for (int i = 0; i < 3; ++i)
        for (int j = 0; j < 3; ++j)
            for (int k = 0; k < 3; ++k)
            {
                shdProgram.setUniform1i("i", i);
                shdProgram.setUniform1i("j", j);
                shdProgram.setUniform1i("k", k);

                // if (rotating && (needs_rotation(hit_index, dir, hit_i, hit_j, hit_k, i, j, k) ||
                    // (i == hit_i && j == hit_j && k == hit_k)))
                if (rotating && i == hit_i && j == hit_j && k == hit_k)
                {
                    // shdProgram.setUniformMatrix4fv("view", view * rotation_view);
                    shdProgram.setUniform1i("is_hit", true);
                    shdProgram.setUniform1f("hit_index", static_cast<float>(hit_index));
                }
                else
                {
                    // shdProgram.setUniformMatrix4fv("view", view);
                    shdProgram.setUniform1i("is_hit", false);
                }

                shdProgram.setUniformMatrix4fv("model", cube.models[i][j][k]);
                shdProgram.setUniformVector3fv("colors", cube.cubes[i][j][k]);

                glDrawArrays(GL_TRIANGLES, 0, 36);
            }
}

void App::update_fps()
{
    static double time_before = 0.0f;
    static int time_counter = 0;

    double time_now = glfwGetTime();
    double mills_passed = (time_now - time_before) * 1000.0;
    time_before = time_now;

    if (time_counter == 30)
    {
        const std::string title = std::to_string(mills_passed);

        glfwSetWindowTitle(window, title.c_str());

        time_counter = 0;
    }

    ++time_counter;
}

bool App::hit_side(const glm::vec3 &mouse_origin, const glm::vec3 &world_ray,
                   int &index, int &hit_i, int &hit_j, int &hit_k)
{
    bool is_anything_hit = false;
    float min_hit_dist = std::numeric_limits<float>::max();

    for (int i = 0; i < 3; ++i)
        for (int j = 0; j < 3; ++j)
            for (int k = 0; k < 3; ++k)
            {
                auto transform_matrix = view * cube.models_full_size[i][j][k];

                constexpr float epsilon = 1e-8;

                float min_dist = std::numeric_limits<float>::max();
                int min_index;
                bool is_hit = false;

                for (int v = 0; v < cube.vec3s.size(); v += 3)
                {
                    auto vertex1 = transform_matrix * glm::vec4(cube.vec3s[v], 1.0f);
                    auto vertex2 = transform_matrix * glm::vec4(cube.vec3s[v + 1], 1.0f);
                    auto vertex3 = transform_matrix * glm::vec4(cube.vec3s[v + 2], 1.0f);

                    float dist = dist_from_ray_to_triangle(mouse_origin, world_ray, vertex1, vertex2, vertex3);

                    // std::cout << dist << '\n';

                    bool non_zero = dist > epsilon;
                    int index = cube.vertices[v * 4 + 3];

                    if (non_zero && dist < min_dist)
                    {
                        is_hit = true;
                        min_dist = dist;
                        min_index = index;
                    }
                }

                // std::cout << "dist for " << i << ' ' << j << ' ' << k << " is " << min_dist << '\n';

                // cube.dists[i][j][k] = is_hit ? min_dist : 0.0f;

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

    // std::cout << "anything hit: " << is_anything_hit << '\n';

    return is_anything_hit;
}

App::~App()
{
    glfwDestroyWindow(window);
    glfwTerminate();
}

void CallbackCaller::error_callback_caller(int error, const char *description)
{
    app->error_callback(error, description);
}

void CallbackCaller::key_callback_caller(GLFWwindow * window, int key, int scancode, int action, int mods)
{
    app->key_callback(window, key, scancode, action, mods);
}

void CallbackCaller::frame_buffer_change_callback_caller(GLFWwindow * window, int width, int height)
{
    app->frame_buffer_change_callback(window, width, height);
}

void CallbackCaller::mouse_callback_caller(GLFWwindow * window, int button, int action, int mods)
{
    app->mouse_callback(window, button, action, mods);
}
