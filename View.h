#pragma once
#include "Widgets.h"
#include "RectangleModel.h"
#include "TextModel.h"
#include "Events.h"
#include "CubeModel.h"
#include "WindowModel.h"
#include "Webcam.h"
#include <opencv2/core.hpp>
#include <tuple>
#include <vector>
#include <cmath>


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
void drawButton(ButtonModel* buttonModel, const WindowModel* windowModel);
void drawTextBox(const TextBoxModel* textBoxModel, const WindowModel* windowModel);
void drawItemBox(const ItemBoxModel* itemBoxModel, const WindowModel* windowModel);
void drawText(const TextModel* textModel, const WindowModel* windowModel);
void drawTextCentered(TextModel* textModel, const RectangleModel* rectModel, const WindowModel* windowModel);
void drawRect(const RectangleModel* rectModel, const WindowModel* windowModel);
void drawCVImage(const WebcamModel* webcamModel, const WindowModel* windowModel);
void drawWebcam(const WebcamModel* webcamModel, const WindowModel* windowModel);
