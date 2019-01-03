#pragma once
#include "TextModel.h"
#include "Events.h"


class TextView
{
public:
    TextView(TextModel* tm) : textModel(tm) {}

    void draw() const;

private:
    TextModel* textModel;
};

class TextController
{
public:
    TextController(TextModel* tm) : textModel(tm) {}

    void d_change(const DimensionsChangeEvent* e);

private:
    TextModel* textModel;
};