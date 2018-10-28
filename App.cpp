#include "App.h"

void error_callback(int error, const char *description)
{
    std::cerr << "GLFW Error (" << error << ") message: " << description << '\n';
}

void key_callback(GLFWwindow *window, int key, int scancode, int action, int mods)
{
    if (action != GLFW_PRESS)
        return;

    switch (key)
    {
        case GLFW_KEY_ESCAPE:
            glfwSetWindowShouldClose(window, true);
            break;
        case GLFW_KEY_F:
            if (mods & GLFW_MOD_SHIFT)
                ;
            else
                ;
            break;
        case GLFW_KEY_B:
            if (mods & GLFW_MOD_SHIFT)
                ;
            else
                ;
            break;
        case GLFW_KEY_R:
            if (mods & GLFW_MOD_SHIFT)
                ;
            else
                ;
            break;
        case GLFW_KEY_L:
            if (mods & GLFW_MOD_SHIFT)
                ;
            else
                ;
            break;
        case GLFW_KEY_U:
            if (mods & GLFW_MOD_SHIFT)
                ;
            else
                ;
            break;
        case GLFW_KEY_D:
            if (mods & GLFW_MOD_SHIFT)
                ;
            else
                ;
            break;
    }

    if (key == GLFW_KEY_ESCAPE)
        glfwSetWindowShouldClose(window, true);
    else if (key == GLFW_KEY_F)
        ;
}

void mouse_callback(GLFWwindow *window, int button, int action, int mods)
{
    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS)
    {
    }
}

void frame_buffer_change_callback(GLFWwindow *window, int width, int height)
{
    glViewport(0, 0, width, height);
}

float dist_from_ray_to_triangle(const glm::vec3 & origin, const glm::vec3 & dir, const glm::vec3 & v0, const glm::vec3 & v1, const glm::vec3 & v2)
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

App::App(int width, int height) : width(width), height(height)
{
    if (!glfwInit())
    {
        std::cerr << "GLFW init failed\n";
        exit(1);
    }

    glfwSetErrorCallback(error_callback);

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

    glfwSetKeyCallback(window, key_callback);
    glfwSetMouseButtonCallback(window, mouse_callback);
    glfwSetFramebufferSizeCallback(window, frame_buffer_change_callback);

    run();
}

void App::run()
{
    glClearColor(0.1f, 0.2f, 0.4f, 1.0f);
    glViewport(0, 0, width, height);
    glEnable(GL_DEPTH_TEST);

    Shader vertex(GL_VERTEX_SHADER, R"(D:\Egor\projects\cpp\Graphics_Experiments\Rubiks_Cube\shaders\standardVertex.glsl)");
    Shader fragment(GL_FRAGMENT_SHADER, R"(D:\Egor\projects\cpp\Graphics_Experiments\Rubiks_Cube\shaders\standardFragment.glsl)");
    ShaderProgram shdProgram(vertex, fragment);
    auto cube = Cube();

    VBO triVBO;
    VAO triVAO;

    triVBO.generate();
    triVAO.generate();

    triVBO.bind();
    triVBO.setStaticData(cube.vertices);

    triVAO.bind();
    triVAO.setAttribPointer(0, 4, false, 4, 0);
    triVAO.enableAttribute(0);

    triVAO.bind();
    triVBO.bind();

    shdProgram.use();

    glm::mat3 colors_i{
        0.0f, 1.0f, 0.0f, // green
        0.0f, 0.0f, 0.0f, // black
        0.0f, 0.0f, 1.0f  // blue
    };

    glm::mat3 colors_j{
        1.0f, 0.0f, 0.0f, // red
        0.0f, 0.0f, 0.0f, // black
        1.0f, 0.5f, 0.0f  // orange
    };

    glm::mat3 colors_k{
        1.0f, 1.0f, 1.0f, // white
        0.0f, 0.0f, 0.0f, // black
        1.0f, 1.0f, 0.0f  // yellow
    };

    std::vector<float> colors{
        1.0f, 1.0f, 0.0f, // yellow
        1.0f, 1.0f, 1.0f, // white
        1.0f, 0.0f, 0.0f, // red
        1.0f, 0.5f, 0.0f, // orange
        0.0f, 0.0f, 1.0f, // blue
        0.0f, 1.0f, 0.0f  // green
    };

    shdProgram.setUniformMatrix3fv("colors_i", colors_i);
    shdProgram.setUniformMatrix3fv("colors_j", colors_j);
    shdProgram.setUniformMatrix3fv("colors_k", colors_k);
    shdProgram.setUniformVector3fv("colors", colors);

    glm::mat4 view(1.0f);
    glm::mat4 projection(1.0f);
    glm::mat4 model(1.0f);
    
    view = glm::translate(view, glm::vec3(0.0f, 0.0f, -3.0f));
    projection = glm::perspective(glm::radians(45.0f), static_cast<float>(width) / height, 0.1f, 100.0f);
    //projection = glm::ortho(-1.0f, 1.0f, -1.0f, 1.0f, 0.1f, 50.0f);

    shdProgram.setUniformMatrix4fv("view", view);
    shdProgram.setUniformMatrix4fv("projection", projection);

    float angle = 0.0f;
    float angle2 = 0.0f;

    cube.rotate('U', false);

    float time_before = 0.0f;
    unsigned long long time_counter = 0;

    while (!glfwWindowShouldClose(window))
    {
        int mouse_state = glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT);

        glm::vec3 world_ray, mouse_origin;
        bool pressed = false;

        if (mouse_state == GLFW_PRESS)
        {
            pressed = true;

            double x_pos, y_pos;
            glfwGetCursorPos(window, &x_pos, &y_pos);

            float x = 2.0 * x_pos / width - 1.0;
            float y = 2.0 * y_pos / height - 1.0;
            y = -y;

            mouse_origin = glm::vec3(0.0f, 0.0f, 0.0f);

            auto eye_ray = glm::inverse(projection) * glm::vec4(x, y, -1.0f, 1.0f);
            eye_ray.z = -1.0f;
            eye_ray.w = 0.0f;

            world_ray = std::move(glm::normalize(glm::vec3(glm::inverse(view) * eye_ray)));

            // std::cout << "LMB pressed"
                // << " ; x = " << world_ray.x
                // << " ; y = " << world_ray.y
                // << " ; z = " << world_ray.z << '\n';
        }

        bool is_anything_hit = false;
        float min_hit_dist = std::numeric_limits<float>::max();
        int min_hit_index;
        int min_hit_i, min_hit_j, min_hit_k;

        for (int i = 0; i < 3; ++i)
            for (int j = 0; j < 3; ++j)
                for (int k = 0; k < 3; ++k)
                {
                    if (false)
                    {
                        model = glm::rotate(glm::mat4(1.0f), glm::radians(angle), glm::vec3(1.0f, 1.0f, 0.0f));
                        model = glm::rotate(model, glm::radians(angle2), glm::vec3(0.0f, 1.0f, 0.0f));
                    }
                    else
                    {
                        model = glm::rotate(glm::mat4(1.0f), glm::radians(angle), glm::vec3(1.0f, 1.0f, 0.0f));
                    }

                    model = glm::translate(model, glm::vec3(0.35f * (i - 1), 0.35f * (j - 1), 0.35f * (k - 1)));
                    glm::mat4 model_full_size = model;

                    model = glm::scale(model, glm::vec3(1.0f / 3.0f));
                    model_full_size = glm::scale(model_full_size, glm::vec3(0.35f));

                    auto transform_matrix = view * model_full_size;

                    cube.models[i][j][k] = std::move(model);
                    cube.models_full_size[i][j][k] = std::move(model_full_size);

                    if (pressed)
                    {
                        constexpr float epsilon = 1e-8;

                        float min_dist = std::numeric_limits<float>::max();
                        float min_index = 0.0f;
                        bool is_hit = false;

                        for (int v = 0; v < cube.vec3s.size(); v += 3)
                        {
                            auto vertex1 = transform_matrix * glm::vec4(cube.vec3s[v], 1.0f);
                            auto vertex2 = transform_matrix * glm::vec4(cube.vec3s[v + 1], 1.0f);
                            auto vertex3 = transform_matrix * glm::vec4(cube.vec3s[v + 2], 1.0f);

                            float dist = dist_from_ray_to_triangle(mouse_origin, world_ray, vertex1, vertex2, vertex3);
                            bool non_zero = dist > epsilon;
                            int index = cube.vertices[v * 4 + 3];

                            if (non_zero && dist < min_dist)
                            {
                                is_hit = true;
                                min_dist = dist;
                                min_index = cube.vertices[v * 4 + 3];
                            }
                        }

                        if (is_hit && min_dist < min_hit_dist)
                        {
                            min_hit_dist = min_dist;
                            min_hit_i = i;
                            min_hit_j = j;
                            min_hit_k = k;
                            min_hit_index = min_index;
                            is_anything_hit = is_hit;
                        }

                        // std::cout << i << ' ' << j << ' ' << k << ' ' << max_dist << '\n';
                    }
                }

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        for (int i = 0; i < 3; ++i)
            for (int j = 0; j < 3; ++j)
                for (int k = 0; k < 3; ++k)
                {
                    shdProgram.setUniform1i("i", i);
                    shdProgram.setUniform1i("j", j);
                    shdProgram.setUniform1i("k", k);

                    if (is_anything_hit && i == min_hit_i && j == min_hit_j && k == min_hit_k)
                    {
                        shdProgram.setUniform1i("is_hit", true);
                        shdProgram.setUniform1f("hit_index", static_cast<float>(min_hit_index));
                    }
                    else
                    {
                        shdProgram.setUniform1i("is_hit", false);
                    }

                    shdProgram.setUniformMatrix4fv("model", cube.models[i][j][k]);
                    shdProgram.setUniformVector3fv("colors", cube.cubes[i][j][k]);

                    glDrawArrays(GL_TRIANGLES, 0, 36);
                }

        angle += 1.0f;
        angle2 += 1.0f;

        float time_now = glfwGetTime();
        float mills_passed = (time_now - time_before) * 1000.0f;
        time_before = time_now;

        if (time_counter % 30 == 0)
        {
            const std::string title = std::to_string(mills_passed);

            glfwSetWindowTitle(window, title.c_str());
        }

        ++time_counter;

        glfwSwapBuffers(window);
        glfwPollEvents();
    }
}

App::~App()
{
    glfwDestroyWindow(window);
    glfwTerminate();
}
