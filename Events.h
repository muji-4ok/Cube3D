#pragma once
#include <string>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

struct Event
{
    virtual std::string get_name();
};

struct MouseEvent : public Event
{
    MouseEvent(const glm::vec2 &mouse_pos, bool left, bool right) : mouse_pos(mouse_pos),
                                                                    left_pressed(left),
                                                                    right_pressed(right) {}
    glm::vec2 mouse_pos;
    bool left_pressed;
    bool right_pressed;
};

struct MouseMoveEvent : public MouseEvent
{
    using MouseEvent::MouseEvent;
    std::string get_name() override;
};

struct MouseDownEvent : public MouseEvent
{
    using MouseEvent::MouseEvent;
    std::string get_name() override;
};

struct MouseUpEvent : public MouseEvent
{
    using MouseEvent::MouseEvent;
    std::string get_name() override;
};

struct KeyPressedEvent : public Event
{
    KeyPressedEvent(char k, bool sh, bool c, bool a, bool su) : key(k), shift(sh), control(c), alt(a), super(su) {}

    char key;
    bool shift;
    bool control;
    bool alt;
    bool super;

    std::string get_name() override;
};

struct DimensionsChangeEvent : public Event
{
    DimensionsChangeEvent(int w, int h) : width(w), height(h) {}

    int width;
    int height;

    std::string get_name() override;
};