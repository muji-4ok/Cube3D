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
#include <algorithm>
#include <ft2build.h>
#include FT_FREETYPE_H


struct TextModel
{
    TextModel(const std::string& t, const glm::vec2& p, const glm::vec3& c, float s) :
        position(p), color(c), scale(s)
    {
        setText(t);
    }

    glm::vec2 position;
    glm::vec2 size;
    glm::vec3 color;
    float scale;

    const std::string& getText() const;
    void setText(const std::string& text);

private:
    std::string text;
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
    const std::string vertex_path = R"(shaders\textVertex.glsl)";
    const std::string fragment_path = R"(shaders\textFragment.glsl)";
    const std::string font_path = R"(D:\Egor\projects\cpp\Graphics_Experiments\x64\Debug\fonts\arial.ttf)";
  #else
    const std::string vertex_path = R"(shaders/textVertex.glsl)";
    const std::string fragment_path = R"(shaders/textFragment.glsl)";
    const std::string font_path = R"(fonts/arial.ttf)";
  #endif
#else
    const std::string vertex_path
        = R"(D:\Egor\projects\cpp\Graphics_Experiments\Rubiks_Cube\shaders\textVertex.glsl)";
    const std::string fragment_path
        = R"(D:\Egor\projects\cpp\Graphics_Experiments\Rubiks_Cube\shaders\textFragment.glsl)";
    const std::string font_path = R"(D:\Egor\projects\cpp\Graphics_Experiments\x64\Debug\fonts\arial.ttf)";
#endif

};

