#pragma once
#include "Common.h"
#include "Shader.h"
#include "Buffers.h"
#include "CubeModel.h"
#include "CubeRenderer.h"
#include "Renderer.h"
#include "Events.h"
#include <glad/glad.h>
#include <glfw/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <opencv2/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <vector>
#include <deque>
#include <iostream>
#include <string>
#include <utility>
#include <limits>
#include <algorithm>
#include <cmath>
#include <random>
#include <ctime>
#include <memory>

struct App
{
public:
    App(int width, int height);
    ~App();

    void error_callback(int, const char *);
    void key_callback(GLFWwindow *window, int key, int scancode, int action, int mods);
    void frame_buffer_change_callback(GLFWwindow *window, int width, int height);
    void mouse_callback(GLFWwindow *window, int button, int action, int mods);

private:
    void run();

    void prepare();
    void draw();
    void update_fps();
    void get_input();
    void reset_events();

    glm::vec2 get_mouse_pos(double x_pos, double y_pos) const;

    cv::VideoCapture vcap;
    bool with_webcam;

    GLFWwindow *window;
    int width;
    int height;

    std::deque<std::unique_ptr<Event>> eventsQueue;
    State gameState = Interactive;
    CubeModel cubeModel;
    CubeRenderer cubeRenderer;
};

