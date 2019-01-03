#include "InteractiveView.h"


void InteractiveView::draw()
{
    controller->process();
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    textView->draw();
    // cubeView->draw();
}

void InteractiveView::handle_event(Event * e)
{
    controller->handle_event(e);
}

InteractiveController::InteractiveController(CubeModel * cm, TextModel * tm, WindowModel * wm) : windowModel(wm)
{
    cubeController = std::make_unique<InteractiveCubeController>(cm);
    textController = std::make_unique<TextController>(tm);
}

void InteractiveController::handle_event(Event * e)
{
    auto mouse_down = dynamic_cast<const MouseDownEvent*>(e);
    auto mouse_up = dynamic_cast<const MouseUpEvent*>(e);
    auto mouse_move = dynamic_cast<const MouseMoveEvent*>(e);
    auto key_pressed = dynamic_cast<const KeyPressedEvent*>(e);
    auto dimensions_change = dynamic_cast<const DimensionsChangeEvent*>(e);

    if (mouse_down)
    {
        cubeController->m_down(mouse_down);
    }
    else if (mouse_up)
    {
        cubeController->m_up(mouse_up);
    }
    else if (mouse_move)
    {
        cubeController->m_move(mouse_move);
    }
    else if (key_pressed)
    {
        cubeController->k_pressed(key_pressed);

        if (key_pressed->key == static_cast<char>(GLFW_KEY_ESCAPE))
            windowModel->close_window();
    }
    else if (dimensions_change)
    {
        cubeController->d_change(dimensions_change);
        textController->d_change(dimensions_change);
    }
}

void InteractiveController::process()
{
    cubeController->rotate();
}
