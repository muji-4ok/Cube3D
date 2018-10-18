#include <cmath>

#include "GLApp.h"
#include <glad/glad.h>
#include <iostream>
#include <cmath>


GLApp::GLApp(int width, int height, std::string title, int major, int minor) :
    width(width), height(height), running(true)//,
    //window(sf::VideoMode(static_cast<unsigned int>(width), static_cast<unsigned int>(height)), title,
    //       sf::Style::Default,
    //       sf::ContextSettings(24, 0, 0, static_cast<unsigned int>(major), static_cast<unsigned int>(minor),
    //                           sf::ContextSettings::Core))
{
    //window.setVerticalSyncEnabled(true);
    //window.setActive(true);
    //window.setFramerateLimit(60);

    if (!gladLoadGL())
    {
        std::cerr << "Failed to load glad\n";
        exit(1);
    }

    initGL();

    while (running)
    {
        handleEvents();
        update();
        render();
        //window.display();
    }

    cleanUp();
}

void GLApp::render()
{
    glClear(GL_COLOR_BUFFER_BIT);

    standardProgram.use();
    triangleState.bind();

    glDrawElements(GL_TRIANGLE_STRIP, 6, GL_UNSIGNED_INT, nullptr);
//    glDrawArrays(GL_TRIANGLES, 0, 3);
}

void GLApp::handleEvents()
{
    //sf::Event event {};

    //while (window.pollEvent(event))
    //{
    //    if (event.type == sf::Event::Closed)
    //    {
    //        running = false;
    //    }
    //    else if (event.type == sf::Event::Resized)
    //    {
    //        glViewport(0, 0, event.size.width, event.size.height);
    //    }
    //}
}

void GLApp::update()
{
    //if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
    //    running = false;

    //float time = clock.getElapsedTime().asSeconds();
    //auto tweenVal = static_cast<float>((std::sin(time) / 2.0) + 0.5);
    //standardProgram.setUniform1f("tweenVal", tweenVal);
}

void GLApp::initGL()
{
    glClearColor(0.1f, 0.1f, 0.2f, 1.0f);

    std::vector<float> vertices = {
        0.5f,  0.5f, 0.0f,  // top right
        0.5f, -0.5f, 0.0f,  // bottom right
        -0.5f, -0.5f, 0.0f,  // bottom left
        -0.5f,  0.5f, 0.0f   // top left
    };

    std::vector<unsigned int> indices = {
        0, 1, 3,   // first triangle
        1, 2, 3    // second triangle
    };

    Shader vertexShader(GL_VERTEX_SHADER, R"(..\shaders\standardVertex.glsl)");
    Shader fragmentShader(GL_FRAGMENT_SHADER, R"(..\shaders\standardFragment.glsl)");
    standardProgram = ShaderProgram(vertexShader, fragmentShader);

    triangleState.generate();
    trianglePositions.generate();
    triangleIndices.generate();

    triangleState.bind();

    trianglePositions.bind();
    trianglePositions.setData(vertices);

    triangleIndices.bind();
    triangleIndices.setData(indices);

    //triangleState.setAttribPointer<float>(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);
    VBO::unbind();
    VAO::unbind();
    EBO::unbind();

    //vertexShader.cleanUp();
    //fragmentShader.cleanUp();
}

void GLApp::cleanUp()
{
    //standardProgram.cleanUp();
}
