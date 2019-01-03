#pragma once
#include "Events.h"
#include "Common.h"
#include <glad/glad.h>
#include <glfw/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <deque>
#include <memory>
#include <iostream>
#include <string>


class WindowModel
{
public:
    WindowModel(int width, int height);
    ~WindowModel();

    void add_event(Event* e);
    Event* pop_event();
    Event* peek_event() const;
    bool event_queue_empty() const;
    void close_window();
    void swap_buffers();
    void poll_events();
    void set_title(const std::string &title);

    glm::vec2 get_mouse_pos() const;
    bool is_left_mb_pressed() const;
    bool is_right_mb_pressed() const;

    GLFWwindow *window;
    int width;
    int height;
    State appState = Interactive;
    bool closed = false;

private:
    std::deque<std::unique_ptr<Event>> eventQueue;
};
