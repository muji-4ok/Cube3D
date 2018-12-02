#include "App.h"

App * CallbackCaller::app = nullptr;

void App::error_callback(int error, const char *description)
{
    std::cerr << "GLFW Error (" << error << ") message: " << description << '\n';
}

void App::key_callback(GLFWwindow *window, int key, int scancode, int action, int mods)
{
    if (action == GLFW_RELEASE)
    {
        bool shift = (mods & GLFW_MOD_SHIFT);
        bool control = (mods & GLFW_MOD_CONTROL);
        bool alt = (mods & GLFW_MOD_ALT);
        bool super = (mods & GLFW_MOD_SUPER);

        char k;

        switch (key)
        {
            case GLFW_KEY_U:
                k = 'U';
                break;
            default:
                std::cout << "Unknown key\n";
                return;
        }

        eventsQueue.emplace_back(new KeyPressedEvent(k, shift, control, alt, super));
    }

    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    if (key == GLFW_KEY_ENTER && action == GLFW_PRESS)
        cube.set_solved();

    if (key == GLFW_KEY_R && action == GLFW_PRESS)
        shuffle();

    if (key == GLFW_KEY_U && action == GLFW_PRESS)
    {
        return;
        rot_angle = 0.0f;
        final_angle = mods & GLFW_MOD_SHIFT ? 90.0f : -90.0f;
        auto_rotation_dir = final_angle - rot_angle;
        increment = glm::radians(3.0f);
        rotating = true;
        auto_rotating = true;
        hit_index = 2;
        dir = DIR1;
        turns = mods & GLFW_MOD_SHIFT ? 1 : 3;
        hit_i = 0;
        hit_j = 2;
        hit_k = 0;
        has_dir = false;
        has_rotation_vec = false;
    }
}

void App::mouse_callback(GLFWwindow *window, int button, int action, int mods)
{
    double x_pos, y_pos;
    glfwGetCursorPos(window, &x_pos, &y_pos);
    auto mouse_pos = get_mouse_pos(x_pos, y_pos, width, height);

    bool is_pressed = (action == GLFW_PRESS);
    bool is_left = (button == GLFW_MOUSE_BUTTON_LEFT);
    bool is_right = (button == GLFW_MOUSE_BUTTON_RIGHT);

    if (is_pressed)
        eventsQueue.emplace_back(new MouseDownEvent(mouse_pos, is_left, is_right));
    else
        eventsQueue.emplace_back(new MouseUpEvent(mouse_pos, is_left, is_right));

    if (button == GLFW_MOUSE_BUTTON_LEFT)
    {
        mouse_pressed = (action == GLFW_PRESS);
        if (action == GLFW_RELEASE)
        {
            if (!rotating)
                return;
            auto angle = glm::degrees(accum_rot_angle);
            // std::cout << "frames left: " << angle / 16.67f << '\n';
            int rot_index = get_rot_index(hit_index, dir);
            turns = static_cast<int>(std::round(angle / 90.0f));
            final_angle = glm::radians(90.0f * turns);
            auto_rotating = true;
            auto_rotation_dir = final_angle - accum_rot_angle;
            increment = glm::radians(3.0f);
            turns = (turns > 0 ? 1 : -1) * (std::abs(turns) % 4);
            if (rot_index == 4 || rot_index == 5)
                turns = -turns;
            if (turns == 0 && std::abs(glm::degrees(rot_angle)) > 2.0f)
            {
                turns = rot_angle > 0 ? 1 : -1;
                final_angle = glm::radians(rot_angle > 0 ? 90.0f : -90.0f);
                auto_rotation_dir = final_angle - accum_rot_angle;
                if (rot_index == 4 || rot_index == 5)
                    turns = -turns;
            }
            turns = (turns + 4) % 4;
            // std::cout << "accum_rot_angle: " << accum_rot_angle << '\n';
            // std::cout << "angle: " << angle << '\n';
            // std::cout << "turns: " << turns << '\n';
            // std::cout << "hit: i = " << hit_i << " ; j = " << hit_j << " ; k = " << hit_k << '\n';
            // std::cout << "index: " << hit_index << '\n';
            return;
        }
        if (auto_rotating)
            return;
        accum_rot_angle = 0.0f;
        double x_pos, y_pos;
        glfwGetCursorPos(window, &x_pos, &y_pos);
        last_mouse_pos = get_mouse_pos(x_pos, y_pos, width, height);
        // std::cout << x_pos << ' ' << y_pos << '\n';
        auto world_ray = get_eye_ray(last_mouse_pos, projection, translate_view);
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
        this->has_rotation_vec = false;
        this->rotating = true;
    }
    else if (button == GLFW_MOUSE_BUTTON_RIGHT)
    {
        right_mouse_pressed = (action == GLFW_PRESS);
        original_view = view;
        // if (!right_mouse_pressed && is_cur_good)
            // mat *= cur_mat;
        double x_pos, y_pos;
        glfwGetCursorPos(window, &x_pos, &y_pos);
        last_right_mouse_pos = get_mouse_pos(x_pos, y_pos, width, height);
    }
}

void App::frame_buffer_change_callback(GLFWwindow *window, int width, int height)
{
    eventsQueue.emplace_back(new DimensionsChangeEvent(width, height));
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

float App::get_rotation_angle(int index, int i, int j, int k, Rotation_Dir dir, const glm::vec2 &dir_vec,
                              const glm::vec2 &original_mouse_pos, const glm::vec2 &mouse_pos, glm::vec3 &rotation_vec)
{
    if (!has_dir)
    {
        auto dist = glm::distance(original_mouse_pos, mouse_pos);

        constexpr float epsilon = 1e-4;

        if (dist < epsilon)
            return 0.0f;
        else
            return dist;
    }
    else
    {
        auto mouse_diff = mouse_pos - original_mouse_pos;

        auto dot = 2.0f * glm::dot(dir_vec, mouse_diff);

        // std::cout << "dot: " << dot << '\n';

        if (!has_rotation_vec)
        {
            has_rotation_vec = true;
            rotation_vec = get_rot_vec(hit_index, hit_i, hit_j, hit_k, dir);
        }

        auto rot_index = get_rot_index(index, dir);

        if (needs_fixing(index, rot_index))
            dot = -dot;

        return dot;
    }
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

    // std::cout << "v1: x = " << v1.x << " ; y = " << v1.y << " ; z = " << v1.z << '\n';
    // std::cout << "v2: x = " << v2.x << " ; y = " << v2.y << " ; z = " << v2.z << '\n';
    // std::cout << "v3: x = " << v3.x << " ; y = " << v3.y << " ; z = " << v3.z << '\n';

    auto e1 = v2 - v1;
    auto e2 = v3 - v1;

    return glm::normalize(glm::vec3(glm::inverse(get_mat()) * glm::vec4(glm::cross(e1, e2), 1.0f)));
}

glm::vec3 App::get_rotation_vec(int index)
{
    glm::vec3 v1, v2, v3;
    get_pane_vertices(index, v1, v2, v3);
    return {};
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

Rotation_Dir App::get_dir(int index, const glm::vec2 &original_mouse_pos, const glm::vec2 &mouse_pos, glm::vec2 &dir_vec)
{
    glm::vec3 pane_vert_1, pane_vert_2, pane_vert_3;
    get_pane_vertices(index, pane_vert_1, pane_vert_2, pane_vert_3);

    auto pane_vec_1 = glm::normalize(glm::vec2(pane_vert_2 - pane_vert_1));
    auto pane_vec_2 = glm::normalize(glm::vec2(pane_vert_3 - pane_vert_1));

    auto mouse_diff_vec = mouse_pos - original_mouse_pos;

    // std::cout << "mouse_diff_vec: x = " << mouse_diff_vec.x << " ; y = " << mouse_diff_vec.y << '\n';

    auto dot_1 = std::abs(glm::dot(pane_vec_1, mouse_diff_vec));
    auto dot_2 = std::abs(glm::dot(pane_vec_2, mouse_diff_vec));

    // std::cout << "dot_1: " << dot_1 << ' ' << "dot_2: " << dot_2 << '\n';

    if (std::isnan(dot_1))
    {
        std::cout << "dot is nan while getting dir\n";

        return DIR1;
    }
    else
        if (dot_1 > dot_2)
        {
            dir_vec = pane_vec_1;

            return DIR1;
        }
        else
        {
            dir_vec = pane_vec_2;

            return DIR2;
        }
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

bool App::are_similary_oriented(const glm::vec3 &stable, const glm::vec3 &unstable)
{
    auto cmp = [&stable](const int &ind_1, const int &ind_2) {
        return std::abs(stable[ind_1]) < std::abs(stable[ind_2]);
    };

    auto max_i = std::max({ 0, 1, 2 }, cmp);

    return stable[max_i] * unstable[max_i] > 0;
}

bool App::needs_fixing(int index, int rot_index)
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

glm::vec3 App::get_index_normal(int index)
{
    glm::vec3 v1, v2, v3;
    get_pane_vertices(index, v1, v2, v3);

    auto e1 = v2 - v1;
    auto e2 = v3 - v1;

    return glm::normalize(glm::cross(e1, e2));
}

glm::mat4 App::get_mat()
{
    if (is_cur_good)
        return mat * cur_mat;
    else
        return mat;
}

std::array<std::vector<float>*, 8> App::get_rotating_cubelets(int index, Rotation_Dir dir,
                                                              int hit_i, int hit_j, int hit_k)
{
    std::array<std::vector<float>*, 8> out;
    using Coord = std::tuple<int, int, int>;
    std::array<Coord, 8> coords;
    int c = 0;

    auto count_centers = [](int i, int j, int k) -> int {
        return static_cast<int>(i == 1) + static_cast<int>(j == 1) + static_cast<int>(k == 1);
    };

    bool is_center = needs_rotation(index, dir, hit_i, hit_j, hit_k, 1, 1, 1);

    for (int i = 0; i < 3; ++i)
        for (int j = 0; j < 3; ++j)
            for (int k = 0; k < 3; ++k)
                if (needs_rotation(index, dir, hit_i, hit_j, hit_k, i, j, k))
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

    int rot_index = get_rot_index(index, dir);

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
        out[i] = &cube.cubes[std::get<0>(coords[i])][std::get<1>(coords[i])][std::get<2>(coords[i])];

    return out;

}

Cubelet_Rotation App::get_cubelet_rotation(int index)
{
    switch (index)
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

void App::rotate(int index, Rotation_Dir dir, int turns, int hit_i, int hit_j, int hit_k)
{
    auto cubelets = get_rotating_cubelets(index, dir, hit_i, hit_j, hit_k);
    auto rot_index = get_rot_index(index, dir);

    for (int i = 0; i < turns; ++i)
        cube.rotate(cubelets, get_cubelet_rotation(rot_index));
}

void App::shuffle()
{
    static std::mt19937 r(std::time(0));

    for (int i = 0; i < 25; ++i)
    {
        int index = r() % 6;
        Rotation_Dir dir = r() % 2 ? DIR1 : DIR2;
        int turns = r() % 4;
        int hit_i = r() % 3;
        int hit_j = r() % 3;
        int hit_k = r() % 3;

        rotate(index, dir, turns, hit_i, hit_j, hit_k);
    }
}

void App::get_input()
{
    double x_pos, y_pos;
    glfwGetCursorPos(window, &x_pos, &y_pos);
    auto mouse_pos = get_mouse_pos(x_pos, y_pos, width, height);

    static auto last_mouse_pos = mouse_pos;
    auto mouse_diff = mouse_pos - last_mouse_pos;
    last_mouse_pos = mouse_pos;

    bool left_pressed = glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS;
    bool right_pressed = glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS;

    if (mouse_diff.x || mouse_diff.y)
        eventsQueue.emplace_back(new MouseMoveEvent(mouse_diff, left_pressed, right_pressed));
}

void App::reset_events()
{
    eventsQueue.clear();
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

App::App(int width, int height) : width(width), height(height), cubeRenderer(&cubeModel)
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
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, true);
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

    if (vcap.open(0))
    {
        vcap.set(CV_CAP_PROP_FRAME_WIDTH, 512);
        vcap.set(CV_CAP_PROP_FRAME_HEIGHT, 512);
        vcap.set(CV_CAP_PROP_FPS, 1);
        with_webcam = true;
    }
    else
    {
        std::cerr << "Failed to find webcam\n";
        with_webcam = false;
    }

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
        reset_events();
        get_input();
        glfwPollEvents();
    }
}

void App::prepare()
{
    glClearColor(0.1f, 0.2f, 0.4f, 1.0f);
    glViewport(0, 0, width, height);
    glEnable(GL_DEPTH_TEST);

    // std::string square_vertex_path = R"(D:\Egor\projects\cpp\Graphics_Experiments\Rubiks_Cube\shaders\squareVertex.glsl)";
    // std::string square_fragment_path = R"(D:\Egor\projects\cpp\Graphics_Experiments\Rubiks_Cube\shaders\squareFragment.glsl)";
    // squareVertex = std::move(Shader(GL_VERTEX_SHADER, square_vertex_path));
    // squareFragment = std::move(Shader(GL_FRAGMENT_SHADER, square_fragment_path));
    // squareShdProgram = std::move(ShaderProgram(squareVertex, squareFragment));
    // squareVBO.generate();
    // squareVAO.generate();
    // squareVBO.bind();
    // squareVBO.setStaticData(square_vertices);
    // squareVAO.bind();
    // squareVAO.setAttribPointer(0, 3, false, 3, 0);
    // squareVAO.enableAttribute(0);
    // squareVAO.bind();
    // squareVBO.bind();
    // VAO::unbind();
    std::string vertex_path = R"(D:\Egor\projects\cpp\Graphics_Experiments\Rubiks_Cube\shaders\standardVertex.glsl)";
    std::string fragment_path = R"(D:\Egor\projects\cpp\Graphics_Experiments\Rubiks_Cube\shaders\standardFragment.glsl)";
    // std::string vertex_path = R"(standardVertex.glsl)";
    // std::string fragment_path = R"(standardFragment.glsl)";
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
    VAO::unbind();
    shdProgram.use();
    view = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -3.0f));
    mat = glm::rotate(mat, glm::radians(25.0f), glm::vec3(1.0f, 0.0f, 0.0f));
    mat = glm::rotate(mat, glm::radians(45.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    projection = glm::perspective(glm::radians(45.0f), static_cast<float>(width) / height, 0.1f, 100.0f);
    shdProgram.setUniformMatrix4fv("view", view);
    shdProgram.setUniformMatrix4fv("projection", projection);

    cubeModel.generate(width, height);
}

void App::calculate()
{
    double x_pos, y_pos;
    glfwGetCursorPos(window, &x_pos, &y_pos);
    auto mouse_pos = get_mouse_pos(x_pos, y_pos, width, height);

    is_cur_good = false;

    if (right_mouse_pressed)
    {
        auto normal_mouse_pos = glm::normalize(glm::vec3(glm::inverse(view) * glm::vec4(0.0f, 0.0f, -1.0f, 1.0f)));
        auto diff = mouse_pos - last_right_mouse_pos;
        auto normal_last_right_mouse_pos = glm::normalize(glm::vec3(glm::inverse(view) * glm::vec4(diff, -1.0f, 1.0f)));

        auto angle = std::acos(glm::dot(normal_mouse_pos, normal_last_right_mouse_pos));
        auto camera_rot_vec = glm::normalize(glm::cross(normal_mouse_pos, normal_last_right_mouse_pos));

        angle += glm::length(diff);

        cur_mat = glm::rotate(glm::mat4(1.0f), angle, camera_rot_vec);

        constexpr float epsilon = 1e-3;

        if (std::abs(angle) > epsilon)
        {
            // std::cout << "normal_mouse: x = " << normal_mouse_pos.x << " ; y = " << normal_mouse_pos.y << " ; z = " << normal_mouse_pos.z << '\n';
            // std::cout << "normal_last_mouse: x = " << normal_last_right_mouse_pos.x << " ; y = " << normal_last_right_mouse_pos.y << " ; z = " << normal_last_right_mouse_pos.z << '\n';
            // std::cout << "rot_vec: x = " << camera_rot_vec.x << " ; y = " << camera_rot_vec.y << " ; z = " << camera_rot_vec.z << '\n';
            mat *= cur_mat;
        }

        last_right_mouse_pos = mouse_pos;
    }

    view = glm::mat4(1.0f);
    view = glm::translate(view, glm::vec3(0.0f, 0.0f, -3.0f));
    view *= get_mat();

    shdProgram.setUniformMatrix4fv("view", view);

    static glm::vec2 dir_vec;
    static glm::vec3 rotation_vec = glm::vec3(1.0f, 0.0f, 0.0f);

    if (auto_rotating)
    {
        if (std::abs(final_angle - accum_rot_angle) < increment)
        {
            rotating = false;
            auto_rotating = false;
            accum_rot_angle = 0.0f;
            rotate(hit_index, dir, turns, hit_i, hit_j, hit_k);
        }
        else
        {
            rot_angle = std::copysign(increment, auto_rotation_dir);
            accum_rot_angle += rot_angle;
            increment += glm::radians(0.8f);
        }
    }
    else
    {
        rot_angle = get_rotation_angle(hit_index, hit_i, hit_j, hit_k, dir,
                                       dir_vec, last_mouse_pos, mouse_pos, rotation_vec);
        accum_rot_angle += rot_angle;
    }

    // std::cout << "accum_rot_angle: " << accum_rot_angle << '\n';

    if (!has_dir && rot_angle)
    {
        dir = get_dir(hit_index, last_mouse_pos, mouse_pos, dir_vec);
        has_dir = true;
    }

    last_mouse_pos = mouse_pos;

    // std::cout << "right_mouse_pressed: " << right_mouse_pressed << '\n';
    // std::cout << "dir_vec: x = " << dir_vec.x << " ; y = " << dir_vec.y << '\n';
    // std::cout << "rotation_vec: x = " << rotation_vec.x << " ; y = " << rotation_vec.y << " ; z = " << rotation_vec.z << '\n';
    // std::cout << "world_ray: x = " << world_ray.x << " ; y = " << world_ray.y << " ; z = " << world_ray.z << '\n';
    // std::cout << "rot_angle: " << rot_angle << '\n';

    for (int i = 0; i < 3; ++i)
        for (int j = 0; j < 3; ++j)
            for (int k = 0; k < 3; ++k)
            {
                glm::mat4 model = glm::mat4(1.0f);

                if (rotating && App::needs_rotation(hit_index, dir, hit_i, hit_j, hit_k, i, j, k))
                    cube.models_rotate[i][j][k] = glm::rotate(cube.models_rotate[i][j][k], rot_angle, rotation_vec);
                    // model = glm::rotate(model, rot_angle, rotation_vec);
                else
                    // model = glm::mat4(1.0f);
                    cube.models_rotate[i][j][k] = glm::mat4(1.0f);

                model *= cube.models_rotate[i][j][k];

                model = glm::translate(model, glm::vec3(0.35f * (i - 1), 0.35f * (j - 1), 0.35f * (k - 1)));

                auto model_full_size = glm::scale(model, glm::vec3(0.35f));
                model = glm::scale(model, glm::vec3(1.0f / 3.0f));

                cube.models[i][j][k] = std::move(model);
                cube.models_full_size[i][j][k] = std::move(model_full_size);
            }
}

void App::draw()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // squareVAO.bind();
    // squareShdProgram.use();
    // glm::mat4 square_model(1.0f);
    // square_model = glm::translate(square_model, glm::vec3(-0.875f, 0.875f, 0.0f));
    // square_model = glm::scale(square_model, glm::vec3(0.25f));
    // squareShdProgram.setUniformMatrix4fv("model", square_model);
    // glDrawArrays(GL_TRIANGLES, 0, 6);
    // static double rotation_angle = 0.0f;
    // auto model = glm::mat4(1.0f);
    // obj_position = glm::vec3(-4.0f, 5 * std::sin(rotation_angle), -4.0f);
    // model = glm::translate(model, obj_position);
    // model = glm::rotate(model, glm::radians(45.0f), glm::vec3(1.0f, 0.0f, 1.0f));
    // shdProgram.setUniformMatrix4fv("model", model);
    // rotation_angle += 0.03f;
    // glDrawArrays(GL_TRIANGLES, 0, 36);
    // cubeVAO.bind();
    // shdProgram.use();

    if (gameState == Interactive)
    {
        for (const auto &e : eventsQueue)
            cubeRenderer.handle_event(e.get());

        cubeRenderer.draw();
    }
    else if (gameState == Walkthrough)
    {

    }
    else if (gameState == Input)
    {
        if (with_webcam)
        {
            cv::Mat frame;
            vcap >> frame;

            if (frame.empty())
                with_webcam = false;

            cv::imshow("this is you, smile! :)", frame);

            // ESC
            if (cv::waitKey(10) == 27)
            {
                glfwSetWindowShouldClose(window, true);
                with_webcam = false;
            }
        }
    }

    return;

    cubeVAO.bind();
    for (int i = 0; i < 3; ++i)
        for (int j = 0; j < 3; ++j)
            for (int k = 0; k < 3; ++k)
            {
                shdProgram.setUniform1i("i", i);
                shdProgram.setUniform1i("j", j);
                shdProgram.setUniform1i("k", k);
                if (rotating && i == hit_i && j == hit_j && k == hit_k)
                {
                    shdProgram.setUniform1i("is_hit", true);
                    shdProgram.setUniform1f("hit_index", static_cast<float>(hit_index));
                }
                else
                {
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
