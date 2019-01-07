#include "InteractiveView.h"


void InteractiveView::draw()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    InteractiveCubeController cubeController(cubeModel);
    cubeController.rotate();
    drawCube(cubeModel, windowModel);
    drawButton(solveButtonModel, windowModel);
    drawButton(nextButtonModel, windowModel);
    drawButton(prevButtonModel, windowModel);
    drawButton(webcamSwitchButtonModel, windowModel);
    drawTextBox(helpBoxModel, windowModel);
    drawItemBox(instructionsBoxModel, windowModel);
    windowModel->updateFPS();
}

void InteractiveView::mousePress(MouseDownEvent * e)
{
    InteractiveCubeController cubeController(cubeModel);
    MouseDownEvent normalized(*e);
    normalized.mouse_pos = windowModel->toNDC(e->mouse_pos);
    cubeController.m_down(&normalized);

    ButtonController buttonController(solveButtonModel);
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
    InteractiveCubeController cubeController(cubeModel);
    MouseUpEvent normalized(*e);
    normalized.mouse_pos = windowModel->toNDC(e->mouse_pos);
    cubeController.m_up(&normalized);

    ButtonController buttonController(solveButtonModel);
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
    InteractiveCubeController cubeController(cubeModel);
    MouseMoveEvent normalized(*e);
    normalized.mouse_pos = windowModel->diffToNDC(e->mouse_pos);
    cubeController.m_move(&normalized);
}

void InteractiveView::keyPress(KeyPressedEvent * e)
{
    InteractiveCubeController cubeController(cubeModel);
    cubeController.k_pressed(e);

    if (e->key == static_cast<char>(GLFW_KEY_ESCAPE))
        windowModel->closeWindow();
}

void InteractiveView::windowResize(DimensionsChangeEvent * e)
{
}
