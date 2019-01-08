#pragma once
#include "Widgets.h"


struct SolveButtonModel : public ButtonModel
{
    SolveButtonModel(const std::function<void()>& callback) :
        ButtonModel(glm::vec2(0.0f, 0.0f), glm::vec2(200.0f, 100.0f), "Solve",
                    glm::vec2(25.0f, 25.0f), 0.5f, glm::vec3(0.0f, 1.0f, 0.0f),
                    glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f), callback) {}
};

struct InteractiveHelpBoxModel : public TextBoxModel
{
    InteractiveHelpBoxModel() : TextBoxModel(glm::vec2(0.0f, -400.0f), glm::vec2(400.0f, 400.0f),
                                             glm::vec3(0.18f, 0.43f, 0.41f))
    {
        addTextLine(glm::vec2(0.0f, -50.0f), "This is a help box", 0.125f, glm::vec3(1.0f, 1.0f, 1.0f));
    }
};

struct InteractiveNextButtonModel : public ButtonModel
{
    InteractiveNextButtonModel(const std::function<void()>& callback) :
        ButtonModel(glm::vec2(210.0f, 0.0f), glm::vec2(200.0f, 100.0f), "Next",
                    glm::vec2(235.0f, 25.0f), 0.5f, glm::vec3(0.0f, 1.0f, 0.0f),
                    glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f), callback) {}
};

struct InteractivePrevButtonModel : public ButtonModel
{
    InteractivePrevButtonModel(const std::function<void()>& callback) :
        ButtonModel(glm::vec2(420.0f, 0.0f), glm::vec2(200.0f, 100.0f), "Prev",
                    glm::vec2(445.0f, 25.0f), 0.5f, glm::vec3(0.0f, 1.0f, 0.0f),
                    glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f), callback) {}
};

struct InstructionsBoxModel : public ItemBoxModel
{
    typedef ItemBoxModel Base;

    InstructionsBoxModel() :
        ItemBoxModel(glm::vec2(900.0f, 0.0f), glm::vec2(900.0f, 200.0f), glm::vec3(0.18f, 0.43f, 0.41f),
                     15.0f, glm::vec2(100.0f, 100.0f), glm::vec3(0.0f, 1.0f, 0.0f), 30.0f, 30.0f, 0.3f,
                     glm::vec3(1.0f, 1.0f, 1.0f), 20.0f, 20.0f)
    {
    }

    std::deque<char> rotationsQueue;

    std::string restoreItem();
    std::string popItem() override;
    void clearItems() override;
};

struct WebcamSwitchButtonModel : public ButtonModel
{
    WebcamSwitchButtonModel(const std::function<void()>& callback) :
        ButtonModel(glm::vec2(630.0f, 0.0f), glm::vec2(200.0f, 100.0f), "Webcam",
                    glm::vec2(655.0f, 25.0f), 0.5f, glm::vec3(0.0f, 1.0f, 0.0f),
                    glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f), callback) {}
};
