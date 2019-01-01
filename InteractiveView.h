#pragma once
#include <glad/glad.h>
#include <glfw/glfw3.h>
#include "Events.h"
#include "CubeModel.h"
#include "CubeController.h"
#include "WindowModel.h"
#include <memory>


class InteractiveController
{
public:
    InteractiveController(CubeModel* cm);

    void handle_event(Event* e);
    void process();

private:
    std::unique_ptr<InteractiveCubeController> cubeController;

};

class InteractiveView
{
public:
    InteractiveView(WindowModel* wm) : windowModel(wm)
    {
        cubeModel = std::make_unique<CubeModel>();
        controller = std::make_unique<InteractiveController>(cubeModel.get());
    }

    void draw();
    void draw_cube();
    void handle_event(Event* e);

private:
    std::unique_ptr<InteractiveController> controller;
    std::unique_ptr<CubeModel> cubeModel;
    WindowModel* windowModel;

};

