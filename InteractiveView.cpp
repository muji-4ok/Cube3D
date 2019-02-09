#include "InteractiveView.h"


void InteractiveView::draw()
{
    windowModel->setMinSize(100, 100);
    windowModel->setViewport(0.0f, 0.0f, windowModel->screenWidth, windowModel->screenHeight);
    glClearColor(0.0f, 0.0f, 0.15f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    InteractiveCubeController cubeController(cubeModel);
    cubeController.rotate();
    drawCube(cubeModel, windowModel);
    drawButton(fastSolveButtonModel, windowModel);
    // drawButton(optimalSolveButtonModel, windowModel);
    /*
    * Add the draw call here.
    */

    if (!instructionsBoxModel->isEmpty())
    {
        drawButton(nextButtonModel, windowModel);
        drawButton(prevButtonModel, windowModel);
        drawItemBox(instructionsBoxModel, windowModel);
    }

    drawButton(webcamSwitchButtonModel, windowModel);
    drawButton(shuffleButtonModel, windowModel);
    drawTextBox(helpBoxModel, windowModel);
    windowModel->updateFPS();
}

void InteractiveView::mousePress(MouseDownEvent * e)
{
    InteractiveCubeController cubeController(cubeModel);
    MouseDownEvent normalized(*e);
    normalized.mouse_pos = windowModel->toNDC(e->mouse_pos);
    cubeController.m_down(&normalized);

    ButtonController buttonController(fastSolveButtonModel, windowModel);
    buttonController.onMousePress(e);
    // buttonController.setModel(optimalSolveButtonModel);
    // buttonController.onMousePress(e);

    /*
    * Call handlers.
    */

    if (!instructionsBoxModel->isEmpty())
    {
        buttonController.setModel(nextButtonModel);
        buttonController.onMousePress(e);
        buttonController.setModel(prevButtonModel);
        buttonController.onMousePress(e);
    }

    buttonController.setModel(webcamSwitchButtonModel);
    buttonController.onMousePress(e);
    buttonController.setModel(shuffleButtonModel);
    buttonController.onMousePress(e);
}

void InteractiveView::mouseRelease(MouseUpEvent * e)
{
    InteractiveCubeController cubeController(cubeModel);
    MouseUpEvent normalized(*e);
    normalized.mouse_pos = windowModel->toNDC(e->mouse_pos);

    InteractiveRotater rotater(cubeModel);

    if (cubeController.m_up(&normalized))
        if (rotater.is_going_to_rotate_finish_interactive())
            if (instructionsBoxModel->isEmpty())
            {
                rotater.finish_interactive_rotation();
            }
            else
            {
                windowModel->appState = std::move(InteractiveResetPopUp);
                auto angle = this->cubeModel->rotationQueue.get_angle();
                auto hitHeader = this->cubeModel->hitModel.get_header();
                auto lastRotation = this->cubeModel->rotationQueue.get_last_rotation();
                auto f = [this, angle, hitHeader, lastRotation]() {
                    InteractiveRotater rotater(this->cubeModel);
                    rotater.finish_interactive_rotation_snap(angle, hitHeader, lastRotation);
                };
                rotater.finish_interactive_rotation_to_zero();
                *yesFunc = std::move(f);
            }
        else
            rotater.finish_interactive_rotation_to_zero();

    ButtonController buttonController(fastSolveButtonModel, windowModel);
    buttonController.onMouseRelease(e);
    // buttonController.setModel(optimalSolveButtonModel);
    // buttonController.onMouseRelease(e);

    /*
    * Call handlers.
    */

    if (!instructionsBoxModel->isEmpty())
    {
        buttonController.setModel(nextButtonModel);
        buttonController.onMouseRelease(e);
        buttonController.setModel(prevButtonModel);
        buttonController.onMouseRelease(e);
    }

    buttonController.setModel(webcamSwitchButtonModel);
    buttonController.onMouseRelease(e);
    buttonController.setModel(shuffleButtonModel);
    buttonController.onMouseRelease(e);
}

void InteractiveView::mouseMove(MouseMoveEvent * e)
{
    InteractiveCubeController cubeController(cubeModel);
    MouseMoveEvent normalized(*e);
    normalized.mouse_pos = windowModel->diffToNDC(e->mouse_pos);
    cubeController.m_move(&normalized);
}

void InteractiveView::keyPress(KeyPressedEvent * e)
{
    InteractiveCubeController cubeController(cubeModel);
    auto kPressedRes = cubeController.k_pressed(e);

    if (!instructionsBoxModel->isEmpty())
    {
        if (kPressedRes == 1)
        {
            windowModel->appState = std::move(InteractiveResetPopUp);
            // Hack
            auto f = [this]() {
                InteractiveCubeController cubeController(this->cubeModel);
                cubeController.shuffle();
            };
            *yesFunc = std::move(f);
        }
        else if (kPressedRes == 2)
        {
            windowModel->appState = std::move(InteractiveResetPopUp);
            auto e_cp = *e;
            auto f = [this, e_cp]() {
                InteractiveCubeController cubeController(this->cubeModel);
                cubeController.rotate_script(e_cp);
            };
            *yesFunc = std::move(f);
        }
    }
    else
    {
        if (kPressedRes == 1)
        {
            cubeController.shuffle();
        }
        else if (kPressedRes == 2)
        {
            auto e_cp = *e;
            cubeController.rotate_script(e_cp);
        }
    }

    if (e->key == static_cast<char>(GLFW_KEY_ESCAPE))
        windowModel->closeWindow();
}

void InteractiveView::windowResize(DimensionsChangeEvent * e)
{
}

void InteractiveResetPopUpView::draw()
{
    interactiveView->draw();

    windowModel->setViewport(windowModel->screenWidth * 0.25f, windowModel->screenHeight * 0.32f,
                             windowModel->screenWidth * 0.5f, windowModel->screenHeight * 0.35f);
    glEnable(GL_SCISSOR_TEST);
    glScissor(windowModel->screenWidth * 0.25f, windowModel->screenHeight * 0.32f,
              windowModel->screenWidth * 0.5f, windowModel->screenHeight * 0.35f);
    glClearColor(0.0f, 0.0f, 0.32f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    drawTextBox(textBoxModel, windowModel);
    drawButton(yesButtonModel, windowModel);
    drawButton(noButtonModel, windowModel);
    glDisable(GL_SCISSOR_TEST);
}

void InteractiveResetPopUpView::mousePress(MouseDownEvent * e)
{
    ButtonController buttonController(yesButtonModel, windowModel);
    buttonController.onMousePress(e);
    buttonController.setModel(noButtonModel);
    buttonController.onMousePress(e);
}

void InteractiveResetPopUpView::mouseRelease(MouseUpEvent * e)
{
    ButtonController buttonController(yesButtonModel, windowModel);
    buttonController.onMouseRelease(e);
    buttonController.setModel(noButtonModel);
    buttonController.onMouseRelease(e);
}

void InteractiveResetPopUpView::mouseMove(MouseMoveEvent * e)
{
}

void InteractiveResetPopUpView::keyPress(KeyPressedEvent * e)
{
    if (e->key == static_cast<char>(GLFW_KEY_ESCAPE))
        windowModel->closeWindow();
}

void InteractiveResetPopUpView::windowResize(DimensionsChangeEvent * e)
{
}

void InteractiveWaitPopUpView::draw()
{
    interactiveView->draw();

    windowModel->setViewport(windowModel->screenWidth * 0.25f, windowModel->screenHeight * 0.32f,
                             windowModel->screenWidth * 0.5f, windowModel->screenHeight * 0.35f);
    glEnable(GL_SCISSOR_TEST);
    glScissor(windowModel->screenWidth * 0.25f, windowModel->screenHeight * 0.32f,
              windowModel->screenWidth * 0.5f, windowModel->screenHeight * 0.35f);
    glClearColor(0.0f, 0.0f, 0.32f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    drawTextBox(textBoxModel, windowModel);
    glDisable(GL_SCISSOR_TEST);
}

void InteractiveWaitPopUpView::mousePress(MouseDownEvent * e)
{
}

void InteractiveWaitPopUpView::mouseRelease(MouseUpEvent * e)
{
}

void InteractiveWaitPopUpView::mouseMove(MouseMoveEvent * e)
{
}

void InteractiveWaitPopUpView::keyPress(KeyPressedEvent * e)
{
    if (e->key == static_cast<char>(GLFW_KEY_ESCAPE))
        windowModel->closeWindow();
}

void InteractiveWaitPopUpView::windowResize(DimensionsChangeEvent * e)
{
}
