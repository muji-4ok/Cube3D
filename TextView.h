#pragma once
#include "TextModel.h"
#include "Events.h"
#include <glm/glm.hpp>


class TextView
{
public:
    TextView(TextModel* tm) : textModel(tm) {}

    void draw(const glm::mat4& projection) const;

private:
    TextModel* textModel;
};
