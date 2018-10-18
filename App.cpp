#include "App.h"

void error_callback(int error, const char *description)
{
    std::cerr << "GLFW Error (" << error << ") message: " << description << '\n';
}

void key_callback(GLFWwindow *window, int key, int scancode, int action, int mods)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}

void frame_buffer_change_callback(GLFWwindow *window, int width, int height)
{
    glViewport(0, 0, width, height);
}

App::App(int width, int height) : width(width), height(height)
{
    if (!glfwInit())
    {
        std::cerr << "GLFW init failed\n";
        exit(1);
    }

    glfwSetErrorCallback(error_callback);

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
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

    glfwSetKeyCallback(window, key_callback);
    glfwSetFramebufferSizeCallback(window, frame_buffer_change_callback);

    run();
}

void App::run()
{
    glClearColor(0.1f, 0.2f, 0.4f, 1.0f);
    glViewport(0, 0, width, height);

    Shader vertex(GL_VERTEX_SHADER, R"(D:\Egor\projects\cpp\Graphics_Experiments\Rubiks_Cube\shaders\standardVertex.glsl)");
    Shader fragment(GL_FRAGMENT_SHADER, R"(D:\Egor\projects\cpp\Graphics_Experiments\Rubiks_Cube\shaders\standardFragment.glsl)");
    ShaderProgram shdProgram(vertex, fragment);

    std::vector<float> vertices{
        0.5f, 0.5f, 0.0f,
        0.5f, -0.5f, 0.0f,
        -0.5f, -0.5f, 0.0f,
        -0.5f,  0.5f, 0.0f
    };

    //std::vector<float> vertices{
    //    -0.5f, -0.5f, 0.0f,
    //    0.5f, -0.5f, 0.0f,
    //    0.0f,  0.5f, 0.0f
    //};

    std::vector<unsigned int> indices{
        0, 1, 3,
        1, 2, 3
    };

    VBO triVBO;
    VAO triVAO;
    EBO triEBO;

    triVBO.generate();
    triVAO.generate();
    triEBO.generate();

    triVBO.bind();
    triVBO.setStaticData(vertices);

    triEBO.bind();
    triEBO.setStaticData(indices);

    triVAO.bind();
    triVAO.setAttribPointer(0, 3, false, 3, 0);
    triVAO.enableAttribute(0);

    triVAO.bind();
    triVBO.bind();
    triEBO.bind();

    shdProgram.use();

    while (!glfwWindowShouldClose(window))
    {
        glClear(GL_COLOR_BUFFER_BIT);

        //glDrawArrays(GL_TRIANGLES, 0, 3);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }
}

App::~App()
{
    glfwDestroyWindow(window);
    glfwTerminate();
}
