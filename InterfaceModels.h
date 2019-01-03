#pragma once
#include "RectangleModel.h"
#include "TextModel.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <string>
#include <vector>
#include <memory>
#include <utility>


class TextBoxModel : public RectangleModel
{
public:
    std::string text;
};

class ButtonModel : public TextBoxModel
{
public:
    bool pressed;

};
