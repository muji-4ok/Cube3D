#pragma once
#include "Widgets.h"


struct InputHelpBoxModel
{

};

struct InputNextButtonModel : public ButtonModel
{
    InputNextButtonModel(const std::function<void()>& callback) :
        ButtonModel(glm::vec2(0.22f, 0.055f), glm::vec2(0.2f, 0.0475f), "Next",
                    0.3f, glm::vec3(0.0f, 1.0f, 0.0f),
                    glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f), callback) {}
};

struct InputPrevButtonModel : public ButtonModel
{
    InputPrevButtonModel(const std::function<void()>& callback) :
        ButtonModel(glm::vec2(0.22f, 0.0f), glm::vec2(0.2f, 0.0475f), "Prev",
                    0.3f, glm::vec3(0.0f, 1.0f, 0.0f),
                    glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f), callback) {}
};

struct ReturnButtonModel
{

};

struct DoneButtonModel
{

};

struct ReadButtonModel : public ButtonModel
{
    ReadButtonModel(const std::function<void()>& callback) :
        ButtonModel(glm::vec2(0.0f, 0.0f), glm::vec2(0.2f, 0.1f), "Read",
                    0.25f, glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(1.0f, 0.0f, 0.0f),
                    glm::vec3(1.0f, 1.0f, 1.0f), callback) {}
};