#include "AppController.h"


AppController* CallbackCaller::controller = nullptr;

AppController::AppController(WindowModel* wm) : windowModel(wm)
{
    CallbackCaller::controller = this;

    glfwSetErrorCallback(CallbackCaller::error_callback_caller);
    glfwSetKeyCallback(windowModel->window, CallbackCaller::key_callback_caller);
    glfwSetMouseButtonCallback(windowModel->window, CallbackCaller::mouse_callback_caller);
    glfwSetFramebufferSizeCallback(windowModel->window, CallbackCaller::frame_buffer_change_callback_caller);
}

void AppController::get_mouse_move_events()
{
    auto mouse_pos = windowModel->get_mouse_pos();

    static auto last_mouse_pos = mouse_pos;
    auto mouse_diff = mouse_pos - last_mouse_pos;
    last_mouse_pos = mouse_pos;

    bool left_pressed = windowModel->is_left_mb_pressed();
    bool right_pressed = windowModel->is_left_mb_pressed();

    if (mouse_diff.x || mouse_diff.y)
        windowModel->add_event(new MouseMoveEvent(mouse_diff, left_pressed, right_pressed));
}

void CallbackCaller::error_callback_caller(int error, const char *description)
{
    controller->error_callback(error, description);
}

void CallbackCaller::key_callback_caller(GLFWwindow * window, int key, int scancode, int action, int mods)
{
    controller->key_callback(key, scancode, action, mods);
}

void CallbackCaller::frame_buffer_change_callback_caller(GLFWwindow * window, int width, int height)
{
    controller->frame_buffer_change_callback(width, height);
}

void CallbackCaller::mouse_callback_caller(GLFWwindow * window, int button, int action, int mods)
{
    controller->mouse_callback(button, action, mods);
}

void AppController::error_callback(int error, const char* description)
{
    std::cerr << "GLFW Error (" << error << ") message: " << description << '\n';
}

void AppController::key_callback(int key, int scancode, int action, int mods)
{
    if (action == GLFW_RELEASE)
    {
        bool shift = (mods & GLFW_MOD_SHIFT);
        bool control = (mods & GLFW_MOD_CONTROL);
        bool alt = (mods & GLFW_MOD_ALT);
        bool super = (mods & GLFW_MOD_SUPER);

        char k;

        switch (key)
        {
            case GLFW_KEY_U:
                k = shift ? 'U' : 'u';
                break;
            case GLFW_KEY_D:
                k = shift ? 'D' : 'd';
                break;
            case GLFW_KEY_L:
                k = shift ? 'L' : 'l';
                break;
            case GLFW_KEY_R:
                k = shift ? 'R' : 'r';
                break;
            case GLFW_KEY_F:
                k = shift ? 'F' : 'f';
                break;
            case GLFW_KEY_B:
                k = shift ? 'B' : 'b';
                break;
            case GLFW_KEY_S:
                k = shift ? 'S' : 's';
                break;
            case GLFW_KEY_C:
                k = shift ? 'C' : 'c';
                break;
            case GLFW_KEY_ESCAPE:
                k = GLFW_KEY_ESCAPE;
                break;
            default:
                std::cout << "Unknown key\n";
                return;
        }

        windowModel->add_event(new KeyPressedEvent(k, shift, control, alt, super));
    }
}

void AppController::frame_buffer_change_callback(int width, int height)
{
    windowModel->add_event(new DimensionsChangeEvent(width, height));
    glViewport(0, 0, width, height);
    windowModel->width = width;
    windowModel->height = height;
}

void AppController::mouse_callback(int button, int action, int mods)
{
    auto mouse_pos = windowModel->get_mouse_pos();

    bool is_pressed = (action == GLFW_PRESS);
    bool is_left = (button == GLFW_MOUSE_BUTTON_LEFT);
    bool is_right = (button == GLFW_MOUSE_BUTTON_RIGHT);

    if (is_pressed)
        windowModel->add_event(new MouseDownEvent(mouse_pos, is_left, is_right));
    else
        windowModel->add_event(new MouseUpEvent(mouse_pos, is_left, is_right));
}
