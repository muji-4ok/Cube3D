#pragma once
#include "Events.h"
#include "Common.h"
#include <glad/glad.h>
#include <glfw/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <deque>
#include <memory>
#include <iostream>
#include <string>


enum State
{
    Interactive = 0,
    Input = 1
};

class WindowModel
{
public:
    WindowModel(int width, int height);
    ~WindowModel();

    void addEvent(Event* e);
    Event* popEvent();
    Event* peekEvent() const;
    bool eventQueueEmpty() const;
    void closeWindow();
    void swapBuffers();
    void pollEvents();
    void updateFPS();
    void setTitle(const std::string &title);

    glm::vec2 getMousePos() const;
    glm::vec2 toNDC(const glm::vec2& mousePos) const;
    bool isLeftMbPressed() const;
    bool isRightMbPressed() const;

    void error_callback(int error, const char* description);
    void key_callback(int key, int scancode, int action, int mods);
    void frame_buffer_change_callback(int width, int height);
    void mouse_callback(int button, int action, int mods);

    glm::mat4 perspectiveProjection;
    glm::mat4 orthogonalProjection;
    GLFWwindow *window;
    int width;
    int height;
    State appState = Interactive;
    bool closed = false;

private:
    std::deque<std::unique_ptr<Event>> eventQueue;
    void getMouseMoveEvents();
    void setPerspectiveProjection(float width, float height);
    void setOrthogonalProjection(float width, float height);
};

struct CallbackCaller
{
    static WindowModel *windowModel;

    static void error_callback_caller(int error, const char* description);
    static void key_callback_caller(GLFWwindow *window, int key, int scancode, int action, int mods);
    static void frame_buffer_change_callback_caller(GLFWwindow *window, int width, int height);
    static void mouse_callback_caller(GLFWwindow *window, int button, int action, int mods);
};