#include "InteractiveView.h"


void InteractiveView::draw()
{
    controller->process();
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    solveButton->draw(interfaceModel->projection);
    cubeView->draw();
}

void InteractiveView::handle_event(Event * e)
{
    controller->handle_event(e);
}

InteractiveController::InteractiveController(CubeModel * cm, InterfaceModel * im, SolveButton* sb,
                                             WindowModel * wm) :
    windowModel(wm), interfaceModel(im), solveButton(sb)
{
    cubeController = std::make_unique<InteractiveCubeController>(cm);
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

        if (solveButton->isInside(windowModel->normal_mouse_to_screen(mouse_down->mouse_pos))
            && !solveButton->pressed && mouse_down->left_pressed)
            solveButton->onMousePress();
    }
    else if (mouse_up)
    {
        cubeController->m_up(mouse_up);

        solveButton->onMouseRelease();

        if (solveButton->isInside(windowModel->normal_mouse_to_screen(mouse_up->mouse_pos))
            && solveButton->pressed && mouse_up->left_pressed)
        {

        }
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
        interfaceModel->set_orthogonal_projection(dimensions_change->width, dimensions_change->height);
    }
}

void InteractiveController::process()
{
    cubeController->rotate();
}
