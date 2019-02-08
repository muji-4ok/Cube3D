#include "InteractiveView.h"


void InteractiveView::draw()
{
    windowModel->setViewport(0.0f, 0.0f, windowModel->screenWidth, windowModel->screenHeight);
    glClearColor(0.0f, 0.0f, 0.15f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    InteractiveCubeController cubeController(cubeModel, instructionsBoxModel);
    cubeController.rotate();
    drawCube(cubeModel, windowModel);
    drawButton(fastSolveButtonModel, windowModel);
    drawButton(optimalSolveButtonModel, windowModel);
    drawButton(nextButtonModel, windowModel);
    drawButton(prevButtonModel, windowModel);
    drawButton(webcamSwitchButtonModel, windowModel);
    drawTextBox(helpBoxModel, windowModel);
    drawItemBox(instructionsBoxModel, windowModel);
    windowModel->updateFPS();
}

void InteractiveView::mousePress(MouseDownEvent * e)
{
    InteractiveCubeController cubeController(cubeModel, instructionsBoxModel);
    MouseDownEvent normalized(*e);
    normalized.mouse_pos = windowModel->toNDC(e->mouse_pos);
    cubeController.m_down(&normalized);

    ButtonController buttonController(fastSolveButtonModel, windowModel);
    buttonController.onMousePress(e);
    buttonController.setModel(optimalSolveButtonModel);
    buttonController.onMousePress(e);
    buttonController.setModel(nextButtonModel);
    buttonController.onMousePress(e);
    buttonController.setModel(prevButtonModel);
    buttonController.onMousePress(e);
    buttonController.setModel(webcamSwitchButtonModel);
    buttonController.onMousePress(e);
}

void InteractiveView::mouseRelease(MouseUpEvent * e)
{
    InteractiveCubeController cubeController(cubeModel, instructionsBoxModel);
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
                rotater.finish_interactive_rotation_to_zero();
                windowModel->appState = InteractiveResetPopUp;
            }
        else
            rotater.finish_interactive_rotation_to_zero();

    ButtonController buttonController(fastSolveButtonModel, windowModel);
    buttonController.onMouseRelease(e);
    buttonController.setModel(optimalSolveButtonModel);
    buttonController.onMouseRelease(e);
    buttonController.setModel(nextButtonModel);
    buttonController.onMouseRelease(e);
    buttonController.setModel(prevButtonModel);
    buttonController.onMouseRelease(e);
    buttonController.setModel(webcamSwitchButtonModel);
    buttonController.onMouseRelease(e);
}

void InteractiveView::mouseMove(MouseMoveEvent * e)
{
    InteractiveCubeController cubeController(cubeModel, instructionsBoxModel);
    MouseMoveEvent normalized(*e);
    normalized.mouse_pos = windowModel->diffToNDC(e->mouse_pos);
    cubeController.m_move(&normalized);
}

void InteractiveView::keyPress(KeyPressedEvent * e)
{
    InteractiveCubeController cubeController(cubeModel, instructionsBoxModel);
    cubeController.k_pressed(e);

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
