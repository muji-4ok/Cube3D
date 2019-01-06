#pragma once
#include <glad/glad.h>
#include <glfw/glfw3.h>
#include "Events.h"
#include "CubeModel.h"
#include "TextModel.h"
#include "TextView.h"
#include "CubeController.h"
#include "CubeView.h"
#include "WindowModel.h"
#include <memory>


class InteractiveController
{
public:
    InteractiveController(CubeModel* cm, TextModel* tm, WindowModel* wm);

    void handle_event(Event* e);
    void process();

private:
    std::unique_ptr<InteractiveCubeController> cubeController;
    std::unique_ptr<TextController> textController;
    WindowModel* windowModel;

};

class InteractiveView
{
public:
    InteractiveView(WindowModel* wm) : windowModel(wm)
    {
        cubeModel = std::make_unique<CubeModel>(wm->width, wm->height);
        cubeView = std::make_unique<CubeView>(cubeModel.get());

        auto proj = glm::ortho(0.0f, static_cast<float>(wm->width), 0.0f, static_cast<float>(wm->height));
        textModel = std::make_unique<TextModel>("test123", glm::vec2(50.0f, 50.0f), glm::vec3(1.0f, 1.0f, 1.0f), 1.0f, proj);
        textView = std::make_unique<TextView>(textModel.get());

        controller = std::make_unique<InteractiveController>(cubeModel.get(), textModel.get(), wm);
    }

    void draw();
    void handle_event(Event* e);

private:
    std::unique_ptr<InteractiveController> controller;
    std::unique_ptr<CubeModel> cubeModel;
    std::unique_ptr<CubeView> cubeView;
    std::unique_ptr<TextModel> textModel;
    std::unique_ptr<TextView> textView;
    WindowModel* windowModel;

};

