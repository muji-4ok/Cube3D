#include "WindowModel.h"


WindowModel::WindowModel(int width, int height)
{
    if (!glfwInit())
    {
        std::cerr << "GLFW init failed\n";
        exit(1);
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, true);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    window = glfwCreateWindow(width, height, "Window", nullptr, nullptr);

    if (!window)
    {
        std::cerr << "GLFW window creation failed\n";
        exit(1);
    }

    glfwMakeContextCurrent(window);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cerr << "Failed to initialize GLAD\n";
        exit(1);
    }

    glViewport(0, 0, width, height);
}

void WindowModel::add_event(Event* e)
{
    eventQueue.emplace_back(e);
}

Event * WindowModel::pop_event()
{
    auto e_ptr = std::move(eventQueue.front());
    eventQueue.pop_front();
    return e_ptr.release();
}

Event * WindowModel::peek_event() const
{
    return eventQueue.front().get();
}

bool WindowModel::event_queue_empty() const
{
    return eventQueue.size();
}

void WindowModel::close_window()
{
    closed = true;
    glfwSetWindowShouldClose(window, true);
}

void WindowModel::swap_buffers()
{
    glfwSwapBuffers(window);
}

void WindowModel::poll_events()
{
    glfwPollEvents();
}

void WindowModel::set_title(const std::string & title)
{
    glfwSetWindowTitle(window, title.c_str());
}

glm::vec2 WindowModel::get_mouse_pos() const
{
    double x_pos, y_pos;
    glfwGetCursorPos(window, &x_pos, &y_pos);

    return { static_cast<float>(x_pos), static_cast<float>(y_pos) };
}

bool WindowModel::is_left_mb_pressed() const
{
    return (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS);
}

bool WindowModel::is_right_mb_pressed() const
{
    return (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS);
}
