#pragma once
#include "Widgets.h"
#include <cctype>


struct InteractiveResetPopUpTextBoxModel : public TextBoxModel
{
    InteractiveResetPopUpTextBoxModel() :
        TextBoxModel(glm::vec2(0.0f, 0.35f), glm::vec2(1.0f, 0.65f), glm::vec3(0.0f, 0.0f, 0.32f),
                     glm::vec3(1.0f, 1.0f, 1.0f), "Current calculation\nwill be reset", 0.3f) {}

};

struct InteractiveResetPopUpYesButtonModel : public ButtonModel
{
    InteractiveResetPopUpYesButtonModel(const std::function<void()>& callback) :
        ButtonModel(glm::vec2(0.15f, 0.15f), glm::vec2(0.3f, 0.2f), "Yes", 0.3f, glm::vec3(0.0f, 1.0f, 0.0f),
                    glm::vec3(1.0f, 0.5f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f), callback) {}
};

struct InteractiveResetPopUpNoButtonModel : public ButtonModel
{
    InteractiveResetPopUpNoButtonModel(const std::function<void()>& callback) :
        ButtonModel(glm::vec2(0.55f, 0.15f), glm::vec2(0.3f, 0.2f), "No", 0.3f, glm::vec3(0.0f, 1.0f, 0.0f),
                    glm::vec3(1.0f, 0.5f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f), callback) {}
};

struct FastSolveButtonModel : public ButtonModel
{
    FastSolveButtonModel(const std::function<void()>& callback) :
        ButtonModel(glm::vec2(0.0f, 0.055f), glm::vec2(0.2f, 0.0475f), "Fast Solve",
                    0.25f, glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(1.0f, 0.5f, 0.0f),
                    glm::vec3(0.0f, 0.0f, 0.0f), callback) {}
};

struct OptimalSolveButtonModel : public ButtonModel
{
    OptimalSolveButtonModel(const std::function<void()>& callback) :
        ButtonModel(glm::vec2(0.0f, 0.0f), glm::vec2(0.2f, 0.0475f), "Optimal Solve",
                    0.25f, glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(1.0f, 0.5f, 0.0f),
                    glm::vec3(0.0f, 0.0f, 0.0f), callback) {}
};

struct InteractiveHelpBoxModel : public TextBoxModel
{
    InteractiveHelpBoxModel() : TextBoxModel(glm::vec2(0.0f, -0.1f), glm::vec2(0.2f, 0.1f),
                                             glm::vec3(0.18f, 0.43f, 0.41f), glm::vec3(1.0f, 1.0f, 1.0f),
                                             "VI\nVILL\nVIN", 0.2f) {}
};

struct InteractiveNextButtonModel : public ButtonModel
{
    InteractiveNextButtonModel(const std::function<void()>& callback) :
        ButtonModel(glm::vec2(0.22f, 0.055f), glm::vec2(0.2f, 0.0475f), "Next",
                    0.3f, glm::vec3(0.0f, 1.0f, 0.0f),
                    glm::vec3(1.0f, 0.5f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f), callback) {}
};

struct InteractivePrevButtonModel : public ButtonModel
{
    InteractivePrevButtonModel(const std::function<void()>& callback) :
        ButtonModel(glm::vec2(0.22f, 0.0f), glm::vec2(0.2f, 0.0475f), "Prev",
                    0.3f, glm::vec3(0.0f, 1.0f, 0.0f),
                    glm::vec3(1.0f, 0.5f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f), callback) {}
};

struct InstructionsBoxModel : public ItemBoxModel
{
    typedef ItemBoxModel Base;

    InstructionsBoxModel() :
        ItemBoxModel(glm::vec2(0.44f, 0.0f), glm::vec2(0.54f, 0.1f), glm::vec3(0.0f, 0.0f, 0.32f),
                     glm::vec2(50.0f, 50.0f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(1.0f, 0.5f, 0.0f), 0.4f,
                     glm::vec3(0.0f, 0.0f, 0.0f), 5.0f, 18.0f)
    {
    }

    std::deque<char> rotationsQueue;

    std::string restoreItem();
    std::string popItem() override;
    void clearItems() override;
    bool isEmpty() override;
};

struct InstructionsSanitizer
{
    static char toCharNotation(const std::string& in);
    static std::string toStringNotation(char r);
    static char reverseChar(char r);
};

struct WebcamSwitchButtonModel : public ButtonModel
{
    WebcamSwitchButtonModel(const std::function<void()>& callback) :
        ButtonModel(glm::vec2(0.75f, 0.9f), glm::vec2(0.25f, 0.1f), "Webcam",
                    0.4f, glm::vec3(0.0f, 1.0f, 0.0f),
                    glm::vec3(1.0f, 0.5f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f), callback) {}
};
