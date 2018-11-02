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

        // std::cout << x_pos << ' ' << y_pos << '\n';

        auto world_ray = get_eye_ray(x_pos, y_pos, width, height, projection, view);
        auto mouse_origin = glm::vec3(0.0f, 0.0f, 0.0f);

        // std::cout << world_ray.x << ' ' << world_ray.y << ' ' << world_ray.z << '\n';

        int index;
        int hit_i, hit_j, hit_k;

        if (!hit_side(mouse_origin, world_ray, index, hit_i, hit_j, hit_k))
            return;

        this->hit_index = index;
        this->hit_i = hit_i;
        this->hit_j = hit_j;
        this->hit_k = hit_k;

        this->rotating = true;
        this->rotation_angle = 0.0f;
    }
}

void App::frame_buffer_change_callback(GLFWwindow *window, int width, int height)
{
    glViewport(0, 0, width, height);
    projection = glm::perspective(glm::radians(45.0f), static_cast<float>(width) / height, 0.1f, 100.0f);
    shdProgram.setUniformMatrix4fv("projection", projection);
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

App::App(int width, int height) : width(width), height(height)
{
    if (!glfwInit())
    {
        std::cerr << "GLFW init failed\n";
        exit(1);
    }

    CallbackCaller::app = this;

    glfwSetErrorCallback(CallbackCaller::error_callback_caller);

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
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

    auto rotation_vec = get_rotation_vec(hit_index);

    for (int i = 0; i < 3; ++i)
        for (int j = 0; j < 3; ++j)
            for (int k = 0; k < 3; ++k)
            {
                glm::mat4 model;

                model = glm::rotate(glm::mat4(1.0f), glm::radians(45.0f), glm::vec3(1.0f, 1.0f, 0.0f));

                if (rotating && needs_rotation(hit_index, i, j, k, hit_i, hit_j, hit_k))
                    model = glm::rotate(model, glm::radians(rotation_angle), rotation_vec);
                // else
                    // model = glm::mat4(1.0f);

                model = glm::translate(model, glm::vec3(0.35f * (i - 1), 0.35f * (j - 1), 0.35f * (k - 1)));

                auto model_full_size = glm::scale(model, glm::vec3(0.35f));
                model = glm::scale(model, glm::vec3(1.0f / 3.0f));

                auto transform_matrix = view * model_full_size;

                cube.models[i][j][k] = std::move(model);
                cube.models_full_size[i][j][k] = std::move(model_full_size);
            }

    if (rotating)
        rotation_angle += 1.0f;
}

void App::draw()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    for (int i = 0; i < 3; ++i)
        for (int j = 0; j < 3; ++j)
            for (int k = 0; k < 3; ++k)
            {
                shdProgram.setUniform1i("i", i);
                shdProgram.setUniform1i("j", j);
                shdProgram.setUniform1i("k", k);

                if (rotating && (needs_rotation(hit_index, i, j, k, hit_i, hit_j, hit_k) ||
                    (i == hit_i && j == hit_j && k == hit_k)))
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
                glm::mat4 model;
                model = glm::rotate(glm::mat4(1.0f), glm::radians(45.0f), glm::vec3(1.0f, 1.0f, 0.0f));

                model = glm::translate(model, glm::vec3(0.35f * (i - 1), 0.35f * (j - 1), 0.35f * (k - 1)));

                model = glm::scale(model, glm::vec3(0.35f));

                auto transform_matrix = view * model;

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

                    std::cout << dist << '\n';

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
