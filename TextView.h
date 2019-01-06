#pragma once
#include "TextModel.h"
#include "Events.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>


class TextView
{
public:
    TextView(TextModel* tm) : textModel(tm) {}

    void draw() const;

private:
    TextModel* textModel;
    glm::mat4 projection;

};

class TextController
{
public:
    TextController(TextModel* tm) : textModel(tm) {}

    void d_change(const DimensionsChangeEvent* e);

private:
    TextModel* textModel;
};