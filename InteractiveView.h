#pragma once
#include <glad/glad.h>
#include <glfw/glfw3.h>
#include "Events.h"
#include "CubeModel.h"
#include "TextModel.h"
#include "TextView.h"
#include "RectangleModel.h"
#include "RectangleView.h"
#include "Widgets.h"
#include "CubeController.h"
#include "CubeView.h"
#include "WindowModel.h"
#include <memory>


class InteractiveController
{
public:
    InteractiveController(CubeModel* cm, InterfaceModel* im, SolveButton* sb, WindowModel* wm);

    void handle_event(Event* e);
    void process();

private:
    std::unique_ptr<InteractiveCubeController> cubeController;
    SolveButton* solveButton;
    InterfaceModel* interfaceModel;
    WindowModel* windowModel;

};

class InteractiveView
{
public:
    InteractiveView(WindowModel* wm) : windowModel(wm)
    {
        cubeModel = std::make_unique<CubeModel>(wm->width, wm->height);
        cubeView = std::make_unique<CubeView>(cubeModel.get());

        interfaceModel = std::make_unique<InterfaceModel>();
        interfaceModel->set_orthogonal_projection(wm->width, wm->height);

        solveButton = std::make_unique<SolveButton>();

        controller = std::make_unique<InteractiveController>(cubeModel.get(), interfaceModel.get(),
                                                             solveButton.get(), wm);
    }

    void draw();
    void handle_event(Event* e);

private:
    std::unique_ptr<InteractiveController> controller;
    std::unique_ptr<CubeModel> cubeModel;
    std::unique_ptr<CubeView> cubeView;
    std::unique_ptr<InterfaceModel> interfaceModel;
    std::unique_ptr<SolveButton> solveButton;
    WindowModel* windowModel;

};

