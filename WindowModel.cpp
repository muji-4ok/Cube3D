#include "WindowModel.h"


WindowModel* CallbackCaller::windowModel = nullptr;

void CallbackCaller::error_callback_caller(int error, const char *description)
{
    windowModel->error_callback(error, description);
}

void CallbackCaller::key_callback_caller(GLFWwindow * window, int key, int scancode, int action, int mods)
{
    windowModel->key_callback(key, scancode, action, mods);
}

void CallbackCaller::frame_buffer_change_callback_caller(GLFWwindow * window, int width, int height)
{
    windowModel->frame_buffer_change_callback(width, height);
}

void CallbackCaller::mouse_callback_caller(GLFWwindow * window, int button, int action, int mods)
{
    windowModel->mouse_callback(button, action, mods);
}

void WindowModel::error_callback(int error, const char* description)
{
    std::cerr << "GLFW Error (" << error << ") message: " << description << '\n';
}

void WindowModel::key_callback(int key, int scancode, int action, int mods)
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
                // std::cerr << "Unknown key\n";
                return;
        }

        addEvent(new KeyPressedEvent(k, shift, control, alt, super));
    }
}

void WindowModel::frame_buffer_change_callback(int width, int height)
{
    addEvent(new DimensionsChangeEvent(width, height));
    this->screenWidth = width;
    this->screenHeight = height;
}

void WindowModel::mouse_callback(int button, int action, int mods)
{
    auto mousePos = getMousePos();

    bool isPressed = (action == GLFW_PRESS);
    bool isLeft = (button == GLFW_MOUSE_BUTTON_LEFT);
    bool isRight = (button == GLFW_MOUSE_BUTTON_RIGHT);

    if (isPressed)
        addEvent(new MouseDownEvent(mousePos, isLeft, isRight));
    else
        addEvent(new MouseUpEvent(mousePos, isLeft, isRight));
}

WindowModel::WindowModel(int width, int height) : screenWidth(width), viewportWidth(width), screenHeight(height),
                                                  viewportHeight(height), minScreenWidth(width),
                                                  minScreenHeight(height)
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
    glfwSetWindowSizeLimits(window, minScreenWidth, minScreenHeight, GLFW_DONT_CARE, GLFW_DONT_CARE);

    if (!window)
    {
        std::cerr << "GLFW window creation failed\n";
        exit(1);
    }

    CallbackCaller::windowModel = this;

    glfwSetErrorCallback(CallbackCaller::error_callback_caller);
    glfwSetKeyCallback(window, CallbackCaller::key_callback_caller);
    glfwSetMouseButtonCallback(window, CallbackCaller::mouse_callback_caller);
    glfwSetFramebufferSizeCallback(window, CallbackCaller::frame_buffer_change_callback_caller);

    glfwMakeContextCurrent(window);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cerr << "Failed to initialize GLAD\n";
        exit(1);
    }

    glfwSwapInterval(1);
    glClearColor(0.1f, 0.2f, 0.4f, 1.0f);
    glEnable(GL_DEPTH_TEST);
    setViewport(0.0f, 0.0f, width, height);
}

WindowModel::~WindowModel()
{
    glfwDestroyWindow(window);
    glfwTerminate();
}

void WindowModel::addEvent(Event* e)
{
    eventQueue.emplace_back(e);
}

Event * WindowModel::popEvent()
{
    auto e_ptr = std::move(eventQueue.front());
    eventQueue.pop_front();
    return e_ptr.release();
}

Event * WindowModel::peekEvent() const
{
    return eventQueue.front().get();
}

bool WindowModel::eventQueueEmpty() const
{
    return !eventQueue.size();
}

void WindowModel::closeWindow()
{
    glfwSetWindowShouldClose(window, true);
}

void WindowModel::swapBuffers()
{
    glfwSwapBuffers(window);
}

void WindowModel::pollEvents()
{
    glfwPollEvents();
    getMouseMoveEvents();

    if (eventQueue.size() > 1)
    {
        auto lol = 123;
    }

    // if (eventQueue.size() > 0)
        // std::cerr << "[POLL EVENTS]: size = " << eventQueue.size() << '\n';
}

void WindowModel::updateFPS()
{
    static double timeBefore = 0.0f;
    static int timeCounter = 0;

    double timeNow = glfwGetTime();
    double millsPassed = (timeNow - timeBefore) * 1000.0;
    timeBefore = timeNow;

    if (millsPassed < 1000.0 / 60.0)
    {
        std::this_thread::sleep_for(std::chrono::nanoseconds(
            static_cast<long long>(1e9 / 60.0 - millsPassed * 1e6)
        ));
        millsPassed = 1000.0 / 60.0;
    }

    if (timeCounter == 30)
    {
        const std::string title = std::to_string(millsPassed);
        setTitle(title);
        timeCounter = 0;
    }

    ++timeCounter;
}

bool WindowModel::isClosed() const
{
    return glfwWindowShouldClose(window);
}

bool WindowModel::isFullScreen() const
{
    auto videoMode = glfwGetVideoMode(glfwGetPrimaryMonitor());
    return videoMode->width == screenWidth &&
        std::abs(videoMode->height - screenHeight) <= videoMode->height * 0.08f;
}

void WindowModel::getMouseMoveEvents()
{
    auto mousePos = getMousePos();

    static auto lastMousePos = mousePos;
    auto mouseDiff = mousePos - lastMousePos;
    lastMousePos = mousePos;

    bool leftPressed = isLeftMbPressed();
    bool rightPressed = isRightMbPressed();

    if (mouseDiff.x || mouseDiff.y)
        addEvent(new MouseMoveEvent(mouseDiff, leftPressed, rightPressed));
}

void WindowModel::setPerspectiveProjection(float width, float height)
{
    perspectiveProjection = glm::perspective(glm::radians(45.0f), width / height, 0.1f, 100.0f);
}

void WindowModel::setOrthogonalProjection(float width, float height)
{
    orthogonalProjection = glm::ortho(0.0f, width, 0.0f, height);
}

void WindowModel::setTitle(const std::string & title)
{
    glfwSetWindowTitle(window, title.c_str());
}

void WindowModel::setViewport(float x, float y, float width, float height)
{
    viewportWidth = width;
    viewportHeight = height;
    viewportX = x;
    viewportY = y;
    glViewport(x, y, width, height);
    setOrthogonalProjection(width, height);
    setPerspectiveProjection(width, height);
}

void WindowModel::setAspectRation(int num, int den)
{
    glfwSetWindowAspectRatio(window, num, den);
}

void WindowModel::setMinSize(int width, int height)
{
    minScreenWidth = width;
    minScreenHeight = height;
    glfwSetWindowSizeLimits(window, width, height, GLFW_DONT_CARE, GLFW_DONT_CARE);
}

glm::vec2 WindowModel::getMousePos() const
{
    double x, y;
    glfwGetCursorPos(window, &x, &y);
    y = screenHeight - y;
    return { static_cast<float>(x), static_cast<float>(y) };
}

glm::vec2 WindowModel::toNDC(const glm::vec2 & mousePos) const
{
    float x = mousePos.x / viewportWidth * 2.0f - 1.0f;
    float y = mousePos.y / viewportHeight * 2.0f - 1.0f;
    return { x, y };
}

glm::vec2 WindowModel::diffToNDC(const glm::vec2 & mouseDiff) const
{
    return { static_cast<float>(mouseDiff.x) / viewportWidth, static_cast<float>(mouseDiff.y) / viewportHeight };
}

bool WindowModel::isLeftMbPressed() const
{
    return (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS);
}

bool WindowModel::isRightMbPressed() const
{
    return (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS);
}
