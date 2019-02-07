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

struct CancelButtonModel : public ButtonModel
{
    CancelButtonModel(const std::function<void()>& callback) :
        ButtonModel(glm::vec2(0.58f, 0.0f), glm::vec2(0.2f, 0.1f), "Cancel",
                    0.25f, glm::vec3(0.0f, 1.0f, 0.0f),
                    glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f), callback) {}
};

struct SubmitButtonModel : public ButtonModel
{
    SubmitButtonModel(const std::function<void()>& callback) :
        ButtonModel(glm::vec2(0.8f, 0.0f), glm::vec2(0.2f, 0.1f), "Submit",
                    0.25f, glm::vec3(0.0f, 1.0f, 0.0f),
                    glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f), callback) {}
};

struct ReadButtonModel : public ButtonModel
{
    ReadButtonModel(const std::function<void()>& callback) :
        ButtonModel(glm::vec2(0.0f, 0.0f), glm::vec2(0.2f, 0.1f), "Read",
                    0.25f, glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(1.0f, 0.0f, 0.0f),
                    glm::vec3(1.0f, 1.0f, 1.0f), callback) {}
};

struct InputResetButtonModel : ButtonModel
{
    InputResetButtonModel(const std::function<void()>& callback) :
        ButtonModel(glm::vec2(0.44f, 0.0f), glm::vec2(0.2f, 0.1f), "Reset",
                    0.3f, glm::vec3(0.0f, 1.0f, 0.0f),
                    glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f), callback) {}
};
