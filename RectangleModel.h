#pragma once
#include "Shader.h"
#include "Buffers.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <string>
#include <vector>
#include <memory>
#include <utility>


class RectangleModel
{
public:
    RectangleModel(const glm::vec3& p, const glm::vec3& c, const glm::vec2& s) :
        position(p), color(c), scale(s) {};

    glm::vec3 position;
    glm::vec3 color;
    glm::vec2 scale;

};

class RectangleModelOpenGLData
{
public:
    static RectangleModelOpenGLData& instance()
    {
        static RectangleModelOpenGLData self;
        return self;
    }

    RectangleModelOpenGLData(const RectangleModelOpenGLData&) = delete;
    RectangleModelOpenGLData(RectangleModelOpenGLData&&) = delete;
    RectangleModelOpenGLData& operator= (const RectangleModelOpenGLData&) = delete;
    RectangleModelOpenGLData& operator= (RectangleModelOpenGLData&&) = delete;

    const std::vector<float> raw_vertices {
        -0.5f, -0.5f, // Bottom-Left
         0.5f, -0.5f, // Bottom-Right
         0.5f,  0.5f, // Top-Right
         0.5f,  0.5f, // Top-Right
        -0.5f,  0.5f, // Top-Left
        -0.5f, -0.5f  // Bottom-Left
    };

    ShaderProgram shdProgram;
    VBO rectVBO;
    VAO rectVAO;

private:
    RectangleModelOpenGLData();
    ~RectangleModelOpenGLData() {}

#ifdef NDEBUG
  #ifdef _WIN32
    const std::string vertex_path
        = R"(shaders\rectVertex.glsl)";
    const std::string fragment_path
        = R"(shaders\rectFragment.glsl)";
  #else
    const std::string vertex_path
        = R"(shaders/rectVertex.glsl)";
    const std::string fragment_path
        = R"(shaders/rectFragment.glsl)";
  #endif
#else
     const std::string vertex_path
         = R"(D:\Egor\projects\cpp\Graphics_Experiments\Rubiks_Cube\shaders\rectVertex.glsl)";
     const std::string fragment_path
         = R"(D:\Egor\projects\cpp\Graphics_Experiments\Rubiks_Cube\shaders\rectFragment.glsl)";
#endif

};

