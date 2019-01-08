#pragma once
#include "View.h"
#include "CubeModel.h"
#include "WindowModel.h"
#include "InputInterface.h"
#include "Events.h"
#include "CubeController.h"
#include "Webcam.h"


class InputView : public View
{
public:
    InputView(CubeModel* cm, WebcamModel* webm, WindowModel* winm, InputHelpBoxModel* hbm,
              InputNextButtonModel* nbm, InputPrevButtonModel* pbm, ReturnButtonModel* retbm,
              DoneButtonModel* dbm, ReadButtonModel* readbm)
        : cubeModel(cm), webcamModel(webm), windowModel(winm), helpBoxModel(hbm), nextButtonModel(nbm),
        prevButtonModel(pbm), returnButtonModel(retbm), doneButtonModel(dbm), readButtonModel(readbm) {}

    void draw() override;
    void mousePress(MouseDownEvent* e) override;
    void mouseRelease(MouseUpEvent* e) override;
    void mouseMove(MouseMoveEvent* e) override;
    void keyPress(KeyPressedEvent* e) override;
    void windowResize(DimensionsChangeEvent* e) override;

private:
    CubeModel* cubeModel;
    WebcamModel* webcamModel;
    WindowModel* windowModel;
    InputHelpBoxModel* helpBoxModel;
    InputNextButtonModel* nextButtonModel;
    InputPrevButtonModel* prevButtonModel;
    ReturnButtonModel* returnButtonModel;
    DoneButtonModel* doneButtonModel;
    ReadButtonModel* readButtonModel;
};

