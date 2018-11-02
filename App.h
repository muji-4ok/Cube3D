#pragma once
#include "Shader.h"
#include "Buffers.h"
#include "Cube.h"
#include <glad/glad.h>
#include <glfw/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <vector>
#include <iostream>
#include <string>
#include <utility>
#include <limits>
#include <algorithm>
#include <cmath>

class App
{
public:
    App(int width, int height);
    ~App();

    void error_callback(int, const char *);
    void key_callback(GLFWwindow *window, int key, int scancode, int action, int mods);
    void frame_buffer_change_callback(GLFWwindow *window, int width, int height);
    void mouse_callback(GLFWwindow *window, int button, int action, int mods);

private:
    void run();

    void prepare();
    void calculate();
    void draw();
    void update_fps();

    bool hit_side(const glm::vec3 &mouse_origin, const glm::vec3 &world_ray,
                  int &index, int &hit_i, int &hit_j, int &hit_k);

    GLFWwindow *window;
    const int width;
    const int height;

    Shader vertex;
    Shader fragment;
    ShaderProgram shdProgram;
    VBO cubeVBO;
    VAO cubeVAO;

    Cube cube;
    glm::mat4 projection;
    glm::mat4 view;

    bool is_anything_hit = false;

    bool rotating = false;
    float rotation_angle = 0.0f;

    int hit_index;
    int hit_i, hit_j, hit_k;

    bool mouse_pressed = false;
};

struct CallbackCaller
{
    static App *app;

    static void error_callback_caller(int, const char *);
    static void key_callback_caller(GLFWwindow *window, int key, int scancode, int action, int mods);
    static void frame_buffer_change_callback_caller(GLFWwindow *window, int width, int height);
    static void mouse_callback_caller(GLFWwindow *window, int button, int action, int mods);
};

float dist_from_ray_to_triangle(const glm::vec3 &origin, const glm::vec3 &dir,
                                const glm::vec3 &v0, const glm::vec3 &v1, const glm::vec3 &v2);
glm::vec3 get_eye_ray(double x_pos, double y_pos, int width, int height,
                      const glm::mat4 &projection, const glm::mat4 &view);
bool needs_rotation(int index, int i, int j, int k, int hit_i, int hit_j, int hit_k);
glm::vec3 get_rotation_vec(int index);
