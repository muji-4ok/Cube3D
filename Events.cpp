#include "Events.h"

std::string Event::get_name()
{
    return "Event";
}

std::string MouseMoveEvent::get_name()
{
    return "MouseMoveEvent";
}

std::string MouseDownEvent::get_name()
{
    return "MouseDownEvent";
}

std::string MouseUpEvent::get_name()
{
    return "MouseUpEvent";
}

std::string KeyPressedEvent::get_name()
{
    return "KeyPressedEvent";
}

std::string DimensionsChangeEvent::get_name()
{
    return "DimensionsChangeEvent";
}
