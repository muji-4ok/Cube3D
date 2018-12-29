#pragma once
#include "WindowModel.h"
#include <glad/glad.h>
#include <glfw/glfw3.h>


class AppController
{
public:
    AppController(WindowModel* wm);

    void get_mouse_move_events();

    void error_callback(int error, const char* description);
    void key_callback(int key, int scancode, int action, int mods);
    void frame_buffer_change_callback(int width, int height);
    void mouse_callback(int button, int action, int mods);

private:
    WindowModel* windowModel;
};

struct CallbackCaller
{
    static AppController *controller;

    static void error_callback_caller(int error, const char* description);
    static void key_callback_caller(GLFWwindow *window, int key, int scancode, int action, int mods);
    static void frame_buffer_change_callback_caller(GLFWwindow *window, int width, int height);
    static void mouse_callback_caller(GLFWwindow *window, int button, int action, int mods);
};

