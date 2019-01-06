#include "InteractiveView.h"


void InteractiveView::draw()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    drawCube(cubeModel, windowModel);
    windowModel->updateFPS();
}

void InteractiveView::mousePress(MouseDownEvent * e)
{
}

void InteractiveView::mouseRelease(MouseUpEvent * e)
{
}

void InteractiveView::mouseMove(MouseMoveEvent * e)
{
}

void InteractiveView::keyPress(KeyPressedEvent * e)
{
}

void InteractiveView::windowResize(DimensionsChangeEvent * e)
{
}
