#pragma once
#include <glad/glad.h>
#include <glfw/glfw3.h>
#include "Shader.h"
#include "Buffers.h"
#include "Common.h"
#include "Cubelet.h"
#include "RotationQueue.h"
#include <array>
#include <vector>
#include <memory>
#include <tuple>


class CubeModel
{
public:
    CubeModel(int width, int height);

    void reset_rotations();

    RotationQueue rotationQueue;
    HitModel hitModel;

    std::array<std::array<std::array<Cubelet, 3>, 3>, 3> cubelets;

    glm::mat4 projection;
    glm::mat4 view;
    glm::mat4 rotation_view;
    glm::mat4 translation_view;
};

class CubeModelOpenGLData
{
public:
    static CubeModelOpenGLData& instance()
    {
        static CubeModelOpenGLData self;
        return self;
    }

    CubeModelOpenGLData(const CubeModelOpenGLData&) = delete;
    CubeModelOpenGLData(CubeModelOpenGLData&&) = delete;
    CubeModelOpenGLData& operator= (const CubeModelOpenGLData&) = delete;
    CubeModelOpenGLData& operator= (CubeModelOpenGLData&&) = delete;

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

    ShaderProgram shdProgram;
    VBO cubeVBO;
    VAO cubeVAO;

private:
    CubeModelOpenGLData();
    ~CubeModelOpenGLData() {}

#ifdef NDEBUG
  #ifdef _WIN32
    const std::string vertex_path
        = R"(shaders\standardVertex.glsl)";
    const std::string fragment_path
        = R"(shaders\standardFragment.glsl)";
  #else
    const std::string vertex_path
        = R"(shaders/standardVertex.glsl)";
    const std::string fragment_path
        = R"(shaders/standardFragment.glsl)";
  #endif
#else
     const std::string vertex_path
         = R"(D:\Egor\projects\cpp\Graphics_Experiments\Rubiks_Cube\shaders\standardVertex.glsl)";
     const std::string fragment_path
         = R"(D:\Egor\projects\cpp\Graphics_Experiments\Rubiks_Cube\shaders\standardFragment.glsl)";
#endif

};