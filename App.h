#pragma once
#include "pch.h"
#include "Shader.h"
#include "Buffers.h"

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

