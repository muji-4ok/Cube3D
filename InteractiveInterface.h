#pragma once
#include "Widgets.h"
#include <cctype>


struct SolveButtonModel : public ButtonModel
{
    SolveButtonModel(const std::function<void()>& callback) :
        ButtonModel(glm::vec2(0.0f, 0.0f), glm::vec2(0.2f, 0.1f), "Solve",
                    0.4f, glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(1.0f, 0.0f, 0.0f),
                    glm::vec3(1.0f, 1.0f, 1.0f), callback) {}
};

struct InteractiveHelpBoxModel : public TextBoxModel
{
    InteractiveHelpBoxModel() : TextBoxModel(glm::vec2(0.0f, -50.0f), glm::vec2(100.0f, 50.0f),
                                             glm::vec3(0.18f, 0.43f, 0.41f))
    {
        addTextLine(glm::vec2(0.0f, -30.0f), "This is a help box", 0.125f, glm::vec3(1.0f, 1.0f, 1.0f));
    }
};

struct InteractiveNextButtonModel : public ButtonModel
{
    InteractiveNextButtonModel(const std::function<void()>& callback) :
        ButtonModel(glm::vec2(0.22f, 0.055f), glm::vec2(0.2f, 0.0475f), "Next",
                    0.3f, glm::vec3(0.0f, 1.0f, 0.0f),
                    glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f), callback) {}
};

struct InteractivePrevButtonModel : public ButtonModel
{
    InteractivePrevButtonModel(const std::function<void()>& callback) :
        ButtonModel(glm::vec2(0.22f, 0.0f), glm::vec2(0.2f, 0.0475f), "Prev",
                    0.3f, glm::vec3(0.0f, 1.0f, 0.0f),
                    glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f), callback) {}
};

struct InstructionsBoxModel : public ItemBoxModel
{
    typedef ItemBoxModel Base;

    InstructionsBoxModel() :
        ItemBoxModel(glm::vec2(150.0f, 0.0f), glm::vec2(400.0f, 75.0f), glm::vec3(0.18f, 0.43f, 0.41f),
                     10.0f, glm::vec2(50.0f, 50.0f), glm::vec3(0.0f, 1.0f, 0.0f), 15.0f, 10.0f, 0.4,
                     glm::vec3(1.0f, 1.0f, 1.0f), 5.0f, 18.0f)
    {
    }

    std::deque<char> rotationsQueue;

    std::string restoreItem();
    std::string popItem() override;
    void clearItems() override;
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
        ButtonModel(glm::vec2(860.0f, 0.0f), glm::vec2(200.0f, 75.0f), "Webcam",
                    0.4f, glm::vec3(0.0f, 1.0f, 0.0f),
                    glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f), callback) {}
};
