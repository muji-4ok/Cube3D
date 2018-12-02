#pragma once
#include "Events.h"

class Renderer
{
public:
    virtual bool draw() = 0;
    virtual void handle_event(const Event *e) = 0;
};

