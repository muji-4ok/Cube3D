#pragma once
#include "Widgets.h"


struct InputHelpBoxModel
{
};

struct InputNextButtonModel : public ButtonModel
{
    InputNextButtonModel(const std::function<void()>& callback) :
        ButtonModel(glm::vec2(-0.22f, 0.33f), glm::vec2(0.2f, 0.07f), "Next",
                    0.5f, glm::vec3(0.0f, 1.0f, 0.0f),
                    glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f), callback) {}
};

struct InputPrevButtonModel : public ButtonModel
{
    InputPrevButtonModel(const std::function<void()>& callback) :
        ButtonModel(glm::vec2(-0.22f, 0.42f), glm::vec2(0.2f, 0.07f), "Prev",
                    0.5f, glm::vec3(0.0f, 1.0f, 0.0f),
                    glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f), callback) {}
};

struct CancelButtonModel : public ButtonModel
{
    CancelButtonModel(const std::function<void()>& callback) :
        ButtonModel(glm::vec2(-0.66f, 0.04f), glm::vec2(0.2f, 0.07f), "Cancel",
                    0.5f, glm::vec3(0.0f, 1.0f, 0.0f),
                    glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f), callback) {}
};

struct SubmitButtonModel : public ButtonModel
{
    SubmitButtonModel(const std::function<void()>& callback) :
        ButtonModel(glm::vec2(-0.44f, 0.04f), glm::vec2(0.4f, 0.07f), "Submit",
                    0.5f, glm::vec3(0.0f, 1.0f, 0.0f),
                    glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f), callback) {}
};

struct ReadButtonModel : public ButtonModel
{
    ReadButtonModel(const std::function<void()>& callback) :
        ButtonModel(glm::vec2(-0.22f, 0.6f), glm::vec2(0.2f, 0.07f), "Read",
                    0.5f, glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(1.0f, 0.0f, 0.0f),
                    glm::vec3(0.0f, 0.0f, 0.0f), callback) {}
};

struct InputResetButtonModel : public ButtonModel
{
    InputResetButtonModel(const std::function<void()>& callback) :
        ButtonModel(glm::vec2(-0.22f, 0.51f), glm::vec2(0.2f, 0.07f), "Reset",
                    0.5f, glm::vec3(0.0f, 1.0f, 0.0f),
                    glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f), callback) {}
};
