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

private:
    void run();

    GLFWwindow *window;
    const int width;
    const int height;
};

void error_callback(int, const char *);
void key_callback(GLFWwindow *window, int key, int scancode, int action, int mods);
void frame_buffer_change_callback(GLFWwindow *window, int width, int height);
float dist_from_ray_to_triangle(const glm::vec3 &origin, const glm::vec3 &dir,
                                const glm::vec3 &v0, const glm::vec3 &v1, const glm::vec3 &v2);
