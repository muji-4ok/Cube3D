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
    constexpr int width = 800;
    constexpr int height = 600;

    {
        // OptimalSolverInitializer::instance();
    }

    WindowModel windowModel(width, height);
    CubeModel interactiveCubeModel(&windowModel);
    CubeModel inputCubeModel(&windowModel);
    InstructionsBoxModel instructionsBoxModel;
    OptimalSolveButtonModel optimalSolveButtonModel(
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
    FastSolveButtonModel fastSolveButtonModel(
        [&interactiveCubeModel, &instructionsBoxModel]() {
            if (interactiveCubeModel.rotationQueue.is_rotating())
                return;

            FastSolver solver(&interactiveCubeModel);
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
        [&windowModel]() {
            windowModel.appState = Input;
        }
    );
    InputHelpBoxModel inputHelpBoxModel;
    auto inputCubeIndex = []() -> int& {
        static int index = 1;
        return index;
    };
    InputNextButtonModel inputNextButtonModel(
        [&inputCubeIndex, &inputCubeModel]() {
            if (inputCubeModel.rotationQueue.is_rotating())
                return;

            auto& index = inputCubeIndex();
            ScriptRotater rotater(&inputCubeModel);

            switch (index)
            {
                case 0:
                    rotater.rotate_all_script(X, true);
                    index = 5;
                    break;
                case 1:
                    rotater.rotate_all_script(Y, true);
                    index = 3;
                    break;
                case 2:
                    rotater.rotate_all_script(Z, true);
                    index = 4;
                    break;
                case 3:
                    rotater.rotate_all_script(Y, true);
                    index = 0;
                    break;
                case 4:
                    rotater.rotate_all_script(X, false);
                    index = 1;
                    break;
                case 5:
                    rotater.rotate_all_script(Z, true);
                    index = 2;
                    break;
                default:
                    assert(0);
            }
        }
    );
    InputPrevButtonModel inputPrevButtonModel(
        [&inputCubeIndex, &inputCubeModel]() {
            if (inputCubeModel.rotationQueue.is_rotating())
                return;

            auto& index = inputCubeIndex();
            ScriptRotater rotater(&inputCubeModel);

            switch (index)
            {
                case 0:
                    rotater.rotate_all_script(Y, false);
                    index = 3;
                    break;
                case 1:
                    rotater.rotate_all_script(X, true);
                    index = 4;
                    break;
                case 2:
                    rotater.rotate_all_script(Z, false);
                    index = 5;
                    break;
                case 3:
                    rotater.rotate_all_script(Y, false);
                    index = 1;
                    break;
                case 4:
                    rotater.rotate_all_script(Z, false);
                    index = 2;
                    break;
                case 5:
                    rotater.rotate_all_script(X, false);
                    index = 0;
                    break;
                default:
                    assert(0);
            }
        }
    );
    CancelButtonModel cancelButtonModel(
        [&windowModel]() {
            windowModel.appState = Interactive;
        }
    );
    SubmitButtonModel submitButtonModel(
        [&windowModel, &interactiveCubeModel, &inputCubeModel]() {
            if (inputCubeModel.rotationQueue.is_rotating() || !inputCubeModel.is_valid())
                return;

            // Absolute fucking disgusting hack
            FastSolver solver(&inputCubeModel);

            if (!solver.generateSolution().size())
                return;
            // Absolute fucking disgusting hack

            windowModel.appState = Interactive;

            interactiveCubeModel.copy_colors(inputCubeModel);
        }
    );
    InputResetButtonModel inputResetButtonModel(
        [&inputCubeModel, &inputCubeIndex]() {
            if (inputCubeModel.rotationQueue.is_rotating())
                return;

            inputCubeModel.full_reset();
            inputCubeIndex() = 1;
        }
    );
    ReadButtonModel readButtonModel(
        [&webcamModel, &inputCubeModel, &inputCubeIndex]() {
            if (inputCubeModel.rotationQueue.is_rotating())
                return;

            WebcamController webcamController(&webcamModel);
            webcamController.setCubeFace(&inputCubeModel, inputCubeIndex());
        }
    );

    InteractiveView interactiveView(&interactiveCubeModel, &windowModel, &fastSolveButtonModel,
                                    &optimalSolveButtonModel, &interactiveHelpBoxModel, &interactiveNextButtonModel,
                                    &interactivePrevButtonModel, &instructionsBoxModel, &webcamSwitchButtonModel);
    InputView inputView(&inputCubeModel, &webcamModel, &windowModel, &inputHelpBoxModel, &inputNextButtonModel,
                        &inputPrevButtonModel, &cancelButtonModel, &submitButtonModel, &readButtonModel,
                        &inputResetButtonModel);
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
