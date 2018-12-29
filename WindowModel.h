#pragma once
#include "Events.h"
#include "Common.h"
#include <glfw/glfw3.h>
#include <deque>
#include <memory>


class WindowModel
{
public:
    WindowModel();
    ~WindowModel();

    void add_event(Event *);
    void pop_event();
    std::unique_ptr<Event> peek_event() const;
    bool event_queue_empty() const;

private:
    int width;
    int height;
    GLFWwindow *window;
    State appState = Interactive;
    std::deque<std::unique_ptr<Event>> eventQueue;
};

