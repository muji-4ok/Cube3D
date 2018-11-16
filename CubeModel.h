#pragma once
#include "Shader.h"
#include "Buffers.h"
#include <glad/glad.h>
#include <glfw/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <array>
#include <vector>
#include <map>
#include <utility>

class CubeModel
{
public:
    CubeModel();
    ~CubeModel();

    void generate(int width, int height);
    void set_dimensions(int width, int height);
    void rotate(char face, bool reverse);
    // void rotate(...);

private:
    const std::vector<float> raw_vertices{
        // back
        -0.5f, -0.5f, -0.5f, 0.0f,
         0.5f, -0.5f, -0.5f, 0.0f,
         0.5f,  0.5f, -0.5f, 0.0f,
         0.5f,  0.5f, -0.5f, 0.0f,
        -0.5f,  0.5f, -0.5f, 0.0f,
        -0.5f, -0.5f, -0.5f, 0.0f,

        // front
        -0.5f, -0.5f,  0.5f, 1.0f,
         0.5f, -0.5f,  0.5f, 1.0f,
         0.5f,  0.5f,  0.5f, 1.0f,
         0.5f,  0.5f,  0.5f, 1.0f,
        -0.5f,  0.5f,  0.5f, 1.0f,
        -0.5f, -0.5f,  0.5f, 1.0f,

        // left
        -0.5f,  0.5f,  0.5f, 2.0f,
        -0.5f,  0.5f, -0.5f, 2.0f,
        -0.5f, -0.5f, -0.5f, 2.0f,
        -0.5f, -0.5f, -0.5f, 2.0f,
        -0.5f, -0.5f,  0.5f, 2.0f,
        -0.5f,  0.5f,  0.5f, 2.0f,

        // right
         0.5f,  0.5f,  0.5f, 3.0f,
         0.5f,  0.5f, -0.5f, 3.0f,
         0.5f, -0.5f, -0.5f, 3.0f,
         0.5f, -0.5f, -0.5f, 3.0f,
         0.5f, -0.5f,  0.5f, 3.0f,
         0.5f,  0.5f,  0.5f, 3.0f,

         // bottom
        -0.5f, -0.5f, -0.5f, 4.0f,
         0.5f, -0.5f, -0.5f, 4.0f,
         0.5f, -0.5f,  0.5f, 4.0f,
         0.5f, -0.5f,  0.5f, 4.0f,
        -0.5f, -0.5f,  0.5f, 4.0f,
        -0.5f, -0.5f, -0.5f, 4.0f,

        // top
        -0.5f,  0.5f, -0.5f, 5.0f,
         0.5f,  0.5f, -0.5f, 5.0f,
         0.5f,  0.5f,  0.5f, 5.0f,
         0.5f,  0.5f,  0.5f, 5.0f,
        -0.5f,  0.5f,  0.5f, 5.0f,
        -0.5f,  0.5f, -0.5f, 5.0f
    };
    const std::vector<glm::vec3> normals{
        glm::vec3(0.0f, 0.0f, -1.0f),
        glm::vec3(0.0f, 0.0f, 1.0f),
        glm::vec3(-1.0f, 0.0f, 0.0f),
        glm::vec3(1.0f, 0.0f, 0.0f),
        glm::vec3(0.0f, -1.0f, 0.0f),
        glm::vec3(0.0f, 1.0f, 0.0f)
    };
    const std::vector<glm::vec3> vertices{
        // back
        glm::vec3(-0.5f, -0.5f, -0.5f),
        glm::vec3( 0.5f, -0.5f, -0.5f),
        glm::vec3( 0.5f,  0.5f, -0.5f),
        glm::vec3( 0.5f,  0.5f, -0.5f),
        glm::vec3(-0.5f,  0.5f, -0.5f),
        glm::vec3(-0.5f, -0.5f, -0.5f),

        // front
        glm::vec3(-0.5f, -0.5f,  0.5f),
        glm::vec3( 0.5f, -0.5f,  0.5f),
        glm::vec3( 0.5f,  0.5f,  0.5f),
        glm::vec3( 0.5f,  0.5f,  0.5f),
        glm::vec3(-0.5f,  0.5f,  0.5f),
        glm::vec3(-0.5f, -0.5f,  0.5f),

        // left
        glm::vec3(-0.5f,  0.5f,  0.5f),
        glm::vec3(-0.5f,  0.5f, -0.5f),
        glm::vec3(-0.5f, -0.5f, -0.5f),
        glm::vec3(-0.5f, -0.5f, -0.5f),
        glm::vec3(-0.5f, -0.5f,  0.5f),
        glm::vec3(-0.5f,  0.5f,  0.5f),

        // right
        glm::vec3(0.5f,  0.5f,  0.5f),
        glm::vec3(0.5f,  0.5f, -0.5f),
        glm::vec3(0.5f, -0.5f, -0.5f),
        glm::vec3(0.5f, -0.5f, -0.5f),
        glm::vec3(0.5f, -0.5f,  0.5f),
        glm::vec3(0.5f,  0.5f,  0.5f),

         // bottom
        glm::vec3(-0.5f, -0.5f, -0.5f),
        glm::vec3( 0.5f, -0.5f, -0.5f),
        glm::vec3( 0.5f, -0.5f,  0.5f),
        glm::vec3( 0.5f, -0.5f,  0.5f),
        glm::vec3(-0.5f, -0.5f,  0.5f),
        glm::vec3(-0.5f, -0.5f, -0.5f),

        // top
        glm::vec3(-0.5f,  0.5f, -0.5f),
        glm::vec3( 0.5f,  0.5f, -0.5f),
        glm::vec3( 0.5f,  0.5f,  0.5f),
        glm::vec3( 0.5f,  0.5f,  0.5f),
        glm::vec3(-0.5f,  0.5f,  0.5f),
        glm::vec3(-0.5f,  0.5f, -0.5f)
    };

    std::array<std::array<std::array<std::vector<float>, 3>, 3>, 3 > colors;
    std::array<std::array<std::array<glm::mat4, 3>, 3>, 3 > models;
    std::array<std::array<std::array<glm::mat4, 3>, 3>, 3 > models_full_size;

    const std::string vertex_path
        = R"(D:\Egor\projects\cpp\Graphics_Experiments\Rubiks_Cube\shaders\standardVertex.glsl)";
    const std::string fragment_path
        = R"(D:\Egor\projects\cpp\Graphics_Experiments\Rubiks_Cube\shaders\standardFragment.glsl)";
    ShaderProgram shdProgram;
    VBO cubeVBO;
    VAO cubeVAO;

    glm::mat4 projection;
    glm::mat4 view;

    int width;
    int height;
};

