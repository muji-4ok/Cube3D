#pragma once
#include "WindowModel.h"
#include <glad/glad.h>
#include <glfw3.h>


class AppController
{
public:
    AppController(WindowModel* wm);

    void get_mouse_move_events();


private:
    WindowModel* windowModel;
};
