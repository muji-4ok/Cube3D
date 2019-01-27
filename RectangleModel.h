#pragma once
#include "Shader.h"
#include "Buffers.h"
#include "WindowModel.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <string>
#include <vector>
#include <memory>
#include <utility>


struct RectangleModel
{
    RectangleModel(const glm::vec2& pos, const glm::vec3& col, const glm::vec2& size) :
        position(pos), color(col), size(size) {};

    RectangleModel NDCtoScreen(const WindowModel* windowModel) const;

    glm::vec2 position;
    glm::vec2 size;
    glm::vec3 color;
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

