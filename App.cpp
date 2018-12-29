#include "App.h"

void App::error_callback(int error, const char *description)
{
    std::cerr << "GLFW Error (" << error << ") message: " << description << '\n';
}

void App::key_callback(GLFWwindow *window, int key, int scancode, int action, int mods)
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
                glfwSetWindowShouldClose(window, true);
                break;
            default:
                std::cout << "Unknown key\n";
                return;
        }

        eventsQueue.emplace_back(new KeyPressedEvent(k, shift, control, alt, super));
    }
}

void App::mouse_callback(GLFWwindow *window, int button, int action, int mods)
{
    double x_pos, y_pos;
    glfwGetCursorPos(window, &x_pos, &y_pos);
    auto mouse_pos = get_mouse_pos(x_pos, y_pos);

    bool is_pressed = (action == GLFW_PRESS);
    bool is_left = (button == GLFW_MOUSE_BUTTON_LEFT);
    bool is_right = (button == GLFW_MOUSE_BUTTON_RIGHT);

    if (is_pressed)
        eventsQueue.emplace_back(new MouseDownEvent(mouse_pos, is_left, is_right));
    else
        eventsQueue.emplace_back(new MouseUpEvent(mouse_pos, is_left, is_right));
}

void App::frame_buffer_change_callback(GLFWwindow *window, int width, int height)
{
    eventsQueue.emplace_back(new DimensionsChangeEvent(width, height));
    this->width = width;
    this->height = height;
}

void App::get_input()
{
    double x_pos, y_pos;
    glfwGetCursorPos(window, &x_pos, &y_pos);
    auto mouse_pos = get_mouse_pos(x_pos, y_pos);

    static auto last_mouse_pos = mouse_pos;
    auto mouse_diff = mouse_pos - last_mouse_pos;
    last_mouse_pos = mouse_pos;

    bool left_pressed = glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS;
    bool right_pressed = glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS;

    if (mouse_diff.x || mouse_diff.y)
        eventsQueue.emplace_back(new MouseMoveEvent(mouse_diff, left_pressed, right_pressed));
}

void App::reset_events()
{
    eventsQueue.clear();
}

glm::vec2 App::get_mouse_pos(double x_pos, double y_pos) const
{
    float x = 2.0 * x_pos / width - 1.0;
    float y = 2.0 * y_pos / height - 1.0;
    y = -y;
    return { x, y };
}

App::App(int width, int height) : width(width), height(height), cubeRenderer(&cubeModel)
{
    if (!glfwInit())
    {
        std::cerr << "GLFW init failed\n";
        exit(1);
    }

    CallbackCaller::app = this;

    glfwSetErrorCallback(CallbackCaller::error_callback_caller);

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

    glfwSetKeyCallback(window, CallbackCaller::key_callback_caller);
    glfwSetMouseButtonCallback(window, CallbackCaller::mouse_callback_caller);
    glfwSetFramebufferSizeCallback(window, CallbackCaller::frame_buffer_change_callback_caller);

    if (vcap.open(0))
    {
        vcap.set(CV_CAP_PROP_FRAME_WIDTH, 512);
        vcap.set(CV_CAP_PROP_FRAME_HEIGHT, 512);
        vcap.set(CV_CAP_PROP_FPS, 1);
        with_webcam = true;
    }
    else
    {
        std::cerr << "Failed to find webcam\n";
        with_webcam = false;
    }

    run();
}

void App::run()
{
    prepare();

    while (!glfwWindowShouldClose(window))
    {
        draw();
        glfwSwapBuffers(window);
        update_fps();
        reset_events();
        get_input();
        glfwPollEvents();
    }
}

void App::prepare()
{
    glClearColor(0.1f, 0.2f, 0.4f, 1.0f);
    glViewport(0, 0, width, height);
    glEnable(GL_DEPTH_TEST);

    cubeModel.generate(width, height);
}

void App::draw()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    if (gameState == Interactive)
    {
        for (const auto &e : eventsQueue)
            cubeRenderer.handle_event(e.get());

        cubeRenderer.draw();
    }
    else if (gameState == Walkthrough)
    {

    }
    else if (gameState == Input)
    {
        if (with_webcam)
        {
            cv::Mat frame;
            vcap >> frame;

            if (frame.empty())
                with_webcam = false;

            cv::imshow("this is you, smile! :)", frame);

            // ESC
            if (cv::waitKey(10) == 27)
            {
                glfwSetWindowShouldClose(window, true);
                with_webcam = false;
            }
        }
    }
}

void App::update_fps()
{
    static double time_before = 0.0f;
    static int time_counter = 0;

    double time_now = glfwGetTime();
    double mills_passed = (time_now - time_before) * 1000.0;
    time_before = time_now;

    if (time_counter == 30)
    {
        const std::string title = std::to_string(mills_passed);

        glfwSetWindowTitle(window, title.c_str());

        time_counter = 0;
    }

    ++time_counter;
}

App::~App()
{
    glfwDestroyWindow(window);
    glfwTerminate();
}
