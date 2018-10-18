#ifndef OPENGL_LEARNING_GLAPP_H
#define OPENGL_LEARNING_GLAPP_H

#include <string>
#include <vector>
#include <array>
#include "Buffers.h"
#include "Shader.h"


class GLApp
{
public:
    GLApp(int width, int height, std::string title, int major, int minor);

private:
    void initGL();
    void handleEvents();
    void update();
    void render();
    void cleanUp();


    int width;
    int height;
    bool running;
    //sf::Window window;
    //sf::Clock clock;

    EBO triangleIndices;
    VBO trianglePositions;
    VAO triangleState;
    ShaderProgram standardProgram;
};


#endif //OPENGL_LEARNING_GLAPP_H
