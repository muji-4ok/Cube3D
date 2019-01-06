#pragma once
#include "RectangleModel.h"
#include <glm/glm.hpp>


class RectangleView
{
public:
    RectangleView(RectangleModel* rm) : rectangleModel(rm) {}

    void draw(const glm::mat4& projection) const;

private:
    RectangleModel* rectangleModel;
};

