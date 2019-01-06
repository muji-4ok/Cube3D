#pragma once
#include "Events.h"
#include "CubeModel.h"
#include "WindowModel.h"


struct View
{
    virtual void draw() = 0;
    virtual void mousePress(MouseDownEvent* e) = 0;
    virtual void mouseRelease(MouseUpEvent* e) = 0;
    virtual void mouseMove(MouseMoveEvent* e) = 0;
    virtual void keyPress(KeyPressedEvent* e) = 0;
    virtual void windowResize(DimensionsChangeEvent* e) = 0;
};

void drawCube(const CubeModel* cubeModel, const WindowModel* windowModel);
