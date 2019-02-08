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
                    InstructionsBoxModel* ibm, WebcamSwitchButtonModel* wsbm, std::function<void()>* yesFunc,
                    ShuffleButtonModel* sbm) :
        cubeModel(cm), windowModel(wm), fastSolveButtonModel(fsbm), optimalSolveButtonModel(osbm), helpBoxModel(hbm),
        nextButtonModel(nbm), prevButtonModel(pbm), instructionsBoxModel(ibm), webcamSwitchButtonModel(wsbm),
        yesFunc(yesFunc), shuffleButtonModel(sbm) {}

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
    ShuffleButtonModel* shuffleButtonModel;
    InteractiveHelpBoxModel* helpBoxModel;
    InteractiveNextButtonModel* nextButtonModel;
    InteractivePrevButtonModel* prevButtonModel;
    InstructionsBoxModel* instructionsBoxModel;
    WebcamSwitchButtonModel* webcamSwitchButtonModel;
    std::function<void()>* yesFunc;
};

class InteractiveResetPopUpView : public View
{
public:
    InteractiveResetPopUpView(InteractiveView* iv, WindowModel* wm, InteractiveResetPopUpTextBoxModel* tbm,
                        InteractiveResetPopUpYesButtonModel* ybm, InteractiveResetPopUpNoButtonModel* nbm)
        : interactiveView(iv), windowModel(wm), textBoxModel(tbm), yesButtonModel(ybm), noButtonModel(nbm) {}

    void draw() override;
    void mousePress(MouseDownEvent* e) override;
    void mouseRelease(MouseUpEvent* e) override;
    void mouseMove(MouseMoveEvent* e) override;
    void keyPress(KeyPressedEvent* e) override;
    void windowResize(DimensionsChangeEvent* e) override;

private:
    InteractiveView* interactiveView;
    WindowModel* windowModel;
    InteractiveResetPopUpTextBoxModel* textBoxModel;
    InteractiveResetPopUpYesButtonModel* yesButtonModel;
    InteractiveResetPopUpNoButtonModel* noButtonModel;
};

class InteractiveWaitPopUpView : public View
{
public:
    InteractiveWaitPopUpView(InteractiveView* iv, WindowModel* wm, InteractiveWaitPopUpTextBoxModel* tbm)
        : interactiveView(iv), windowModel(wm), textBoxModel(tbm) {}

    void draw() override;
    void mousePress(MouseDownEvent* e) override;
    void mouseRelease(MouseUpEvent* e) override;
    void mouseMove(MouseMoveEvent* e) override;
    void keyPress(KeyPressedEvent* e) override;
    void windowResize(DimensionsChangeEvent* e) override;

private:
    InteractiveView* interactiveView;
    WindowModel* windowModel;
    InteractiveWaitPopUpTextBoxModel* textBoxModel;
};
