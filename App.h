#pragma once
#include "Shader.h"
#include "Buffers.h"
#include <glad/glad.h>
#include <glfw/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <vector>

class App
{
public:
    App(int width, int height);
    ~App();

private:
    void run();

    GLFWwindow *window;
    const int width;
    const int height;

    //// quads
    //const std::vector<float> vertices{
    //    // back
    //    -0.5f, -0.5f, -0.5f, 
    //     0.5f, -0.5f, -0.5f, 
    //     0.5f,  0.5f, -0.5f, 
    //    -0.5f,  0.5f, -0.5f, 

    //    // front
    //    -0.5f, -0.5f,  0.5f, 
    //     0.5f, -0.5f,  0.5f, 
    //     0.5f,  0.5f,  0.5f, 
    //    -0.5f,  0.5f,  0.5f, 

    //    // left
    //    -0.5f,  0.5f,  0.5f, 
    //    -0.5f,  0.5f, -0.5f, 
    //    -0.5f, -0.5f, -0.5f, 
    //    -0.5f, -0.5f,  0.5f, 

    //    // right
    //     0.5f,  0.5f,  0.5f, 
    //     0.5f,  0.5f, -0.5f, 
    //     0.5f, -0.5f, -0.5f, 
    //     0.5f, -0.5f,  0.5f, 

    //     // bottom
    //    -0.5f, -0.5f, -0.5f, 
    //     0.5f, -0.5f, -0.5f, 
    //     0.5f, -0.5f,  0.5f, 
    //    -0.5f, -0.5f,  0.5f, 

    //    // top
    //    -0.5f,  0.5f, -0.5f, 
    //     0.5f,  0.5f, -0.5f, 
    //     0.5f,  0.5f,  0.5f, 
    //    -0.5f,  0.5f,  0.5f, 
    //};

    const std::vector<float> vertices{
        -0.5f, -0.5f, -0.5f, 
         0.5f, -0.5f, -0.5f, 
         0.5f,  0.5f, -0.5f, 
         0.5f,  0.5f, -0.5f, 
        -0.5f,  0.5f, -0.5f, 
        -0.5f, -0.5f, -0.5f, 

        -0.5f, -0.5f,  0.5f, 
         0.5f, -0.5f,  0.5f, 
         0.5f,  0.5f,  0.5f, 
         0.5f,  0.5f,  0.5f, 
        -0.5f,  0.5f,  0.5f, 
        -0.5f, -0.5f,  0.5f, 

        -0.5f,  0.5f,  0.5f, 
        -0.5f,  0.5f, -0.5f, 
        -0.5f, -0.5f, -0.5f, 
        -0.5f, -0.5f, -0.5f, 
        -0.5f, -0.5f,  0.5f, 
        -0.5f,  0.5f,  0.5f, 

         0.5f,  0.5f,  0.5f, 
         0.5f,  0.5f, -0.5f, 
         0.5f, -0.5f, -0.5f, 
         0.5f, -0.5f, -0.5f, 
         0.5f, -0.5f,  0.5f, 
         0.5f,  0.5f,  0.5f, 

        -0.5f, -0.5f, -0.5f, 
         0.5f, -0.5f, -0.5f, 
         0.5f, -0.5f,  0.5f, 
         0.5f, -0.5f,  0.5f, 
        -0.5f, -0.5f,  0.5f, 
        -0.5f, -0.5f, -0.5f, 

        -0.5f,  0.5f, -0.5f, 
         0.5f,  0.5f, -0.5f, 
         0.5f,  0.5f,  0.5f, 
         0.5f,  0.5f,  0.5f, 
        -0.5f,  0.5f,  0.5f, 
        -0.5f,  0.5f, -0.5f
    };
};

void error_callback(int, const char *);
void key_callback(GLFWwindow *window, int key, int scancode, int action, int mods);
void frame_buffer_change_callback(GLFWwindow *window, int width, int height);
