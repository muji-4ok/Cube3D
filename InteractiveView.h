#pragma once
#include "View.h"
#include "CubeModel.h"
#include "WindowModel.h"
#include "InteractiveInterface.h"
#include "Events.h"
#include "CubeController.h"
#include "Widgets.h"


class InteractiveView : public View
{
public:
    InteractiveView(CubeModel* cm, WindowModel* wm, FastSolveButtonModel* fsbm, OptimalSolveButtonModel* osbm,
                    InteractiveHelpBoxModel* hbm, InteractiveNextButtonModel* nbm, InteractivePrevButtonModel* pbm,
                    InstructionsBoxModel* ibm, WebcamSwitchButtonModel* wsbm) :
        cubeModel(cm), windowModel(wm), fastSolveButtonModel(fsbm), optimalSolveButtonModel(osbm), helpBoxModel(hbm),
        nextButtonModel(nbm), prevButtonModel(pbm), instructionsBoxModel(ibm), webcamSwitchButtonModel(wsbm) {}

    void draw() override;
    void mousePress(MouseDownEvent* e) override;
    void mouseRelease(MouseUpEvent* e) override;
    void mouseMove(MouseMoveEvent* e) override;
    void keyPress(KeyPressedEvent* e) override;
    void windowResize(DimensionsChangeEvent* e) override;

private:
    CubeModel* cubeModel;
    WindowModel* windowModel;
    FastSolveButtonModel* fastSolveButtonModel;
    OptimalSolveButtonModel* optimalSolveButtonModel;
    InteractiveHelpBoxModel* helpBoxModel;
    InteractiveNextButtonModel* nextButtonModel;
    InteractivePrevButtonModel* prevButtonModel;
    InstructionsBoxModel* instructionsBoxModel;
    WebcamSwitchButtonModel* webcamSwitchButtonModel;
};