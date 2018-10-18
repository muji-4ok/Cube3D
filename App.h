#pragma once
#include "Shader.h"
#include "Buffers.h"
#include <glad/glad.h>
#include <glfw/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <vector>

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
