#include "InteractiveView.h"


void InteractiveView::draw()
{
    std::cerr << "InteractiveView draw start\n";

    std::cerr << "InteractiveView.controller->process start\n";

    controller->process();

    std::cerr << "InteractiveView.controller->process end\n";

    draw_cube();

    std::cerr << "InteractiveView draw end\n";
}

void InteractiveView::draw_cube()
{
    std::cerr << "InteracriveView.draw_cube start\n";

    cubeModel->cubeVAO.bind();
    cubeModel->shdProgram.use();

    cubeModel->shdProgram.setUniformMatrix4fv("view", cubeModel->view);
    cubeModel->shdProgram.setUniformMatrix4fv("projection", cubeModel->projection);

    for (int i = 0; i < 3; ++i)
        for (int j = 0; j < 3; ++j)
            for (int k = 0; k < 3; ++k)
            {
                cubeModel->shdProgram.setUniform1i("i", i);
                cubeModel->shdProgram.setUniform1i("j", j);
                cubeModel->shdProgram.setUniform1i("k", k);

                auto model = cubeModel->cubelets[i][j][k].rotation * cubeModel->cubelets[i][j][k].translation;

                cubeModel->shdProgram.setUniformMatrix4fv("model", model);
                cubeModel->shdProgram.setUniformVector3fv("colors", cubeModel->cubelets[i][j][k].colors);

                glDrawArrays(GL_TRIANGLES, 0, 36);
            }

    std::cerr << "InteracriveView.draw_cube end\n";
}

void InteractiveView::handle_event(Event * e)
{
    controller->handle_event(e);
}

InteractiveController::InteractiveController(CubeModel * cm)
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
    }
    else if (dimensions_change)
    {
        cubeController->d_change(dimensions_change);
    }
}

void InteractiveController::process()
{
    std::cerr << "InteractiveView.controller.cubeController->rotate start\n";

    cubeController->rotate();

    std::cerr << "InteractiveView.controller.cubeController->rotate end\n";
}
