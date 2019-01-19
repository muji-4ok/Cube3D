// Rubiks_Cube.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#pragma once

#include "View.h"
#include "Events.h"
#include "InteractiveView.h"
#include "InputView.h"
#include "CubeModel.h"
#include "WindowModel.h"
#include "InteractiveInterface.h"
#include "InputInterface.h"
#include "Rotaters.h"
#include "Solver.h"
#include "Webcam.h"
#include <cctype>


int main()
{
    constexpr int width = 640;
    constexpr int height = 480;

    WindowModel windowModel(width, height);
    CubeModel interactiveCubeModel(&windowModel);
    CubeModel inputCubeModel(&windowModel);
    InstructionsBoxModel instructionsBoxModel;
    SolveButtonModel solveButtonModel(
        [&interactiveCubeModel, &instructionsBoxModel]() {
            if (interactiveCubeModel.rotationQueue.is_rotating())
                return;

            OptimalSolver solver(&interactiveCubeModel);
            auto solution = solver.generateSolution();

            instructionsBoxModel.clearItems();

            for (const auto&r : solution)
                instructionsBoxModel.addItem(InstructionsSanitizer::toStringNotation(r));
        }
    );
    InteractiveHelpBoxModel interactiveHelpBoxModel;
    InteractiveNextButtonModel interactiveNextButtonModel(
        [&interactiveCubeModel, &instructionsBoxModel]() {
            if (interactiveCubeModel.rotationQueue.is_rotating())
                return;

            auto res = instructionsBoxModel.popItem();

            if (res.size())
            {
                ScriptRotater rotater(&interactiveCubeModel);
                rotater.rotate_script(InstructionsSanitizer::toCharNotation(res));
            }
        }
    );
    InteractivePrevButtonModel interactivePrevButtonModel(
        [&interactiveCubeModel, &instructionsBoxModel]() {
            if (interactiveCubeModel.rotationQueue.is_rotating())
                return;

            auto res = instructionsBoxModel.restoreItem();

            if (res.size())
            {
                auto r = InstructionsSanitizer::toCharNotation(res);
                r = InstructionsSanitizer::reverseChar(r);
                ScriptRotater rotater(&interactiveCubeModel);
                rotater.rotate_script(r);
            }
        }
    );
    WebcamModel webcamModel(&windowModel);
    WebcamSwitchButtonModel webcamSwitchButtonModel(
        [&windowModel, &webcamModel]() {
            windowModel.appState = Input;
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
    InputView inputView(&inputCubeModel, &webcamModel, &windowModel, &inputHelpBoxModel, &inputNextButtonModel,
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
