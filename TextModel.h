#pragma once
#include "Shader.h"
#include "Texture.h"
#include "Buffers.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <string>
#include <vector>
#include <memory>
#include <utility>
#include <map>
#include <ft2build.h>
#include FT_FREETYPE_H


class TextModel
{
public:
    TextModel(const std::string& t, const glm::vec2& p, const glm::vec3& c, float s, const glm::mat4& pr) :
        text(t), position(p), color(c), scale(s), projection(pr) {}

    glm::mat4 projection;
    std::string text;
    glm::vec2 position;
    glm::vec3 color;
    float scale;
};


struct DrawChar
{
    std::unique_ptr<Texture> texture;
    glm::ivec2 size;
    glm::ivec2 bearing;
    int advance;
};

class TextModelOpenGLData
{
public:
    static TextModelOpenGLData& instance()
    {
        static TextModelOpenGLData self;
        return self;
    }

    TextModelOpenGLData(const TextModelOpenGLData&) = delete;
    TextModelOpenGLData(TextModelOpenGLData&&) = delete;
    TextModelOpenGLData& operator= (const TextModelOpenGLData&) = delete;
    TextModelOpenGLData& operator= (TextModelOpenGLData&&) = delete;

    ShaderProgram shdProgram;
    VBO textVBO;
    VAO textVAO;
    std::map<char, DrawChar> characters;

private:
    TextModelOpenGLData();
    ~TextModelOpenGLData() {}

#ifdef NDEBUG
  #ifdef _WIN32
    const std::string vertex_path
        = R"(shaders\textVertex.glsl)";
    const std::string fragment_path
        = R"(shaders\textFragment.glsl)";
  #else
    const std::string vertex_path
        = R"(shaders/textVertex.glsl)";
    const std::string fragment_path
        = R"(shaders/textFragment.glsl)";
  #endif
#else
     const std::string vertex_path
         = R"(D:\Egor\projects\cpp\Graphics_Experiments\Rubiks_Cube\shaders\textVertex.glsl)";
     const std::string fragment_path
         = R"(D:\Egor\projects\cpp\Graphics_Experiments\Rubiks_Cube\shaders\textFragment.glsl)";
#endif

 #ifdef _WIN32
     const std::string font_path = R"(D:\Egor\projects\cpp\Graphics_Experiments\x64\Debug\fonts\arial.ttf)";
 #else
     const std::string font_path = R"(fonts/arial.ttf)";
 #endif

};

