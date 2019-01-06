#pragma once
#include <string>
#include <vector>
#include <deque>
#include <memory>
#include <utility>
#include <functional>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "TextModel.h"
#include "TextView.h"
#include "RectangleModel.h"
#include "RectangleView.h"


struct InterfaceModel
{
    glm::mat4 projection;

    void set_orthogonal_projection(float width, float height);
};

struct SolveButton
{
    SolveButton();

    glm::vec3 bgColorPressed = glm::vec3(1.0f, 0.0f, 0.0f);
    glm::vec3 bgColorNormal = glm::vec3(0.0f, 1.0f, 0.0f);
    bool pressed = false;

    RectangleModel rectModel;
    TextModel textModel;
    RectangleView rectView;
    TextView textView;

    void draw(const glm::mat4& projection) const;
    bool isInside(const glm::vec2& mouse_pos) const;
    void onMousePress();
    void onMouseRelease();
};