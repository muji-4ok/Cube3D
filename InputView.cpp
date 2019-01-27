#include "InputView.h"


void InputView::draw()
{
    windowModel->setViewport(0.0f, 0.0f, windowModel->screenWidth / 2, windowModel->screenHeight);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    InputCubeController cubeController(cubeModel);
    cubeController.rotate();
    drawCube(cubeModel, windowModel);
    drawButton(readButtonModel, windowModel);
    drawButton(nextButtonModel, windowModel);
    drawButton(prevButtonModel, windowModel);

    windowModel->setViewport(windowModel->screenWidth / 2, 0.0f, windowModel->screenWidth / 2,
                             windowModel->screenHeight);
    WebcamController webcamController(webcamModel);
    webcamController.readFrame();
    webcamController.resizeToFit(windowModel);
    webcamController.updateColors();
    drawWebcam(webcamModel, windowModel);
    windowModel->updateFPS();
}

void InputView::mousePress(MouseDownEvent * e)
{
    //Buttons
    ButtonController buttonController(readButtonModel, windowModel);
    buttonController.onMousePress(e);
    buttonController.setModel(nextButtonModel);
    buttonController.onMousePress(e);
    buttonController.setModel(prevButtonModel);
    buttonController.onMousePress(e);
}

void InputView::mouseRelease(MouseUpEvent * e)
{
    //Buttons
    ButtonController buttonController(readButtonModel, windowModel);
    buttonController.onMouseRelease(e);
    buttonController.setModel(nextButtonModel);
    buttonController.onMouseRelease(e);
    buttonController.setModel(prevButtonModel);
    buttonController.onMouseRelease(e);
}

void InputView::mouseMove(MouseMoveEvent * e)
{
}

void InputView::keyPress(KeyPressedEvent * e)
{
    if (e->key == static_cast<char>(GLFW_KEY_ESCAPE))
        windowModel->closeWindow();
}

void InputView::windowResize(DimensionsChangeEvent * e)
{
}
