// Rubiks_Cube.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "View.h"
#include "Events.h"
#include "InteractiveView.h"
#include "InputView.h"
#include "CubeModel.h"
#include "WindowModel.h"
#include "InteractiveInterface.h"
#include "InputInterface.h"


int main()
{
    constexpr int width = 640;
    constexpr int height = 480;

    WindowModel windowModel(width, height);
    CubeModel interactiveCubeModel(&windowModel);
    CubeModel inputCubeModel(&windowModel);
    SolveButtonModel solveButtonModel(
        [&interactiveCubeModel]() {
            std::cout << "Solve btn pressed\n";
        }
    );
    InteractiveHelpBoxModel interactiveHelpBoxModel;
    InstructionsBoxModel instructionsBoxModel;
    InteractiveNextButtonModel interactiveNextButtonModel(
        [&interactiveCubeModel, &instructionsBoxModel]() {
            std::cout << "Next btn pressed\n";
            instructionsBoxModel.addItem("a");
        }
    );
    InteractivePrevButtonModel interactivePrevButtonModel(
        [&interactiveCubeModel, &instructionsBoxModel]() {
            std::cout << "Prev btn pressed\n";
            instructionsBoxModel.popItem();
        }
    );
    WebcamSwitchButtonModel webcamSwitchButtonModel(
        [&windowModel]() {
            std::cout << "Webcam switch btn pressed\n";
        }
    );
    InputHelpBoxModel inputHelpBoxModel;
    InputNextButtonModel inputNextButtonModel;
    InputPrevButtonModel inputPrevButtonModel;
    ReturnButtonModel returnButtonModel;
    DoneButtonModel doneButtonModel;
    ReadButtonModel readButtonModel;

    InteractiveView interactiveView(&interactiveCubeModel, &windowModel, &solveButtonModel, &interactiveHelpBoxModel,
                                    &interactiveNextButtonModel, &interactivePrevButtonModel, &instructionsBoxModel,
                                    &webcamSwitchButtonModel);
    InputView inputView(&inputCubeModel, &windowModel, &inputHelpBoxModel, &inputNextButtonModel,
                        &inputPrevButtonModel, &returnButtonModel, &doneButtonModel, &readButtonModel);
    View* curView = nullptr;

    while (!windowModel.isClosed())
    {
        if (windowModel.appState == Interactive)
            curView = &interactiveView;
        else if (windowModel.appState == Input)
            curView = &inputView;

        while (!windowModel.eventQueueEmpty())
        {
            auto e = windowModel.popEvent();

            auto mouseDown = dynamic_cast<MouseDownEvent*>(e);
            auto mouseUp = dynamic_cast<MouseUpEvent*>(e);
            auto mouseMove = dynamic_cast<MouseMoveEvent*>(e);
            auto keyPressed = dynamic_cast<KeyPressedEvent*>(e);
            auto dimensionsChange = dynamic_cast<DimensionsChangeEvent*>(e);

            if (mouseDown)
                curView->mousePress(mouseDown);
            else if (mouseUp)
                curView->mouseRelease(mouseUp);
            else if (mouseMove)
                curView->mouseMove(mouseMove);
            else if (keyPressed)
                curView->keyPress(keyPressed);
            else if (dimensionsChange)
                curView->windowResize(dimensionsChange);
            else
                throw std::runtime_error("Incorrect event ptr");
        }

        curView->draw();
        windowModel.swapBuffers();
        windowModel.pollEvents();
    }
    
    return 0;
}
