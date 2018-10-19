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
    glEnable(GL_DEPTH_TEST);

    Shader vertex(GL_VERTEX_SHADER, R"(D:\Egor\projects\cpp\Graphics_Experiments\Rubiks_Cube\shaders\standardVertex.glsl)");
    Shader fragment(GL_FRAGMENT_SHADER, R"(D:\Egor\projects\cpp\Graphics_Experiments\Rubiks_Cube\shaders\standardFragment.glsl)");
    ShaderProgram shdProgram(vertex, fragment);

    //std::vector<float> vertices{
    //    -0.5f, -0.5f, 0.0f,
    //    0.5f, -0.5f, 0.0f,
    //    0.0f,  0.5f, 0.0f
    //};

    //std::vector<unsigned int> indices{
    //    0, 1, 3,
    //    1, 2, 3
    //};

    VBO triVBO;
    VAO triVAO;
    //EBO triEBO;

    triVBO.generate();
    triVAO.generate();
    //triEBO.generate();

    triVBO.bind();
    triVBO.setStaticData(vertices);

    //triEBO.bind();
    //triEBO.setStaticData(indices);

    triVAO.bind();
    triVAO.setAttribPointer(0, 4, false, 4, 0);
    triVAO.enableAttribute(0);

    triVAO.bind();
    triVBO.bind();
    //triEBO.bind();

    shdProgram.use();

    glm::mat3 colors_i{
        0.0f, 1.0f, 0.0f, // green
        0.0f, 0.0f, 0.0f, // black
        0.0f, 0.0f, 1.0f  // blue
    };

    glm::mat3 colors_j{
        1.0f, 0.0f, 0.0f, // red
        0.0f, 0.0f, 0.0f, // black
        1.0f, 0.5f, 0.0f  // orange
    };

    glm::mat3 colors_k{
        1.0f, 1.0f, 1.0f, // white
        0.0f, 0.0f, 0.0f, // black
        1.0f, 1.0f, 0.0f  // yellow
    };

    std::vector<float> colors{
        1.0f, 1.0f, 0.0f, // yellow
        1.0f, 1.0f, 1.0f, // white
        1.0f, 0.0f, 0.0f, // red
        1.0f, 0.5f, 0.0f, // orange
        0.0f, 0.0f, 1.0f, // blue
        0.0f, 1.0f, 0.0f  // green
    };

    shdProgram.setUniformMatrix3fv("colors_i", colors_i);
    shdProgram.setUniformMatrix3fv("colors_j", colors_j);
    shdProgram.setUniformMatrix3fv("colors_k", colors_k);
    shdProgram.setUniformVector3fv("colors", colors);

    glm::mat4 view(1.0f);
    glm::mat4 projection(1.0f);
    glm::mat4 model(1.0f);
    
    view = glm::translate(view, glm::vec3(0.0f, 0.0f, -3.0f));
    projection = glm::perspective(glm::radians(45.0f), static_cast<float>(width) / height, 0.1f, 100.0f);

    shdProgram.setUniformMatrix4fv("view", view);
    shdProgram.setUniformMatrix4fv("projection", projection);

    float angle = 0.0f;

    while (!glfwWindowShouldClose(window))
    {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        for (int i = 0; i < 3; ++i)
            for (int j = 0; j < 3; ++j)
                for (int k = 0; k < 3; ++k)
                {
                    shdProgram.setUniform1i("i", i);
                    shdProgram.setUniform1i("j", j);
                    shdProgram.setUniform1i("k", k);

                    model = glm::rotate(glm::mat4(1.0f), glm::radians(angle), glm::vec3(1.0, 0.3f, 0.5f));
                    model = glm::translate(model, glm::vec3(0.35f * (i - 1), 0.35f * (j - 1), 0.35f * (k - 1)));
                    model = glm::scale(model, glm::vec3(1.0f / 3.0f));
                    shdProgram.setUniformMatrix4fv("model", model);

                    glDrawArrays(GL_TRIANGLES, 0, 36);
                }

        angle += 1.0f;

        //glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }
}

App::~App()
{
    glfwDestroyWindow(window);
    glfwTerminate();
}
