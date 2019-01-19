#include "InteractiveView.h"


void InteractiveView::draw()
{
    windowModel->setViewport(0.0f, 0.0f, windowModel->screenWidth, windowModel->screenHeight);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    InteractiveCubeController cubeController(cubeModel, instructionsBoxModel);
    cubeController.rotate();
    drawCube(cubeModel, windowModel);
    drawButton(solveButtonModel, windowModel);
    drawButton(nextButtonModel, windowModel);
    drawButton(prevButtonModel, windowModel);
    // drawButton(webcamSwitchButtonModel, windowModel);
    // drawTextBox(helpBoxModel, windowModel);
    // drawItemBox(instructionsBoxModel, windowModel);
    windowModel->updateFPS();
}

void InteractiveView::mousePress(MouseDownEvent * e)
{
    InteractiveCubeController cubeController(cubeModel, instructionsBoxModel);
    MouseDownEvent normalized(*e);
    normalized.mouse_pos = windowModel->toNDC(e->mouse_pos);
    cubeController.m_down(&normalized);

    ButtonController buttonController(solveButtonModel, windowModel);
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
    cubeController.m_up(&normalized);

    ButtonController buttonController(solveButtonModel, windowModel);
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
