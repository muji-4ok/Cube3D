#pragma once
#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include "Buffers.h"
#include "Texture.h"
#include "Shader.h"
#include "WindowModel.h"
#include <cmath>
#include <utility>
#include <tuple>
#include <array>


struct Frame
{
    float x;
    float y;
    float w;
    float h;
};

struct WebcamModel
{
    WebcamModel(const WindowModel* windowModel);

    cv::Mat mat;
    Frame frame;
    const float lineW = 5.0f;
    float delim;
    std::array<float, 4> horDelims;
    std::array<float, 4> vertDelims;
    std::array<std::array<Frame, 3>, 3> readRegions;

    void setDelims(const WindowModel* windowModel);
    void setRegions(const WindowModel* windowModel);
    void setFrame(const WindowModel* windowModel);
};

class CVImageModelOpenGLData
{
public:
    static CVImageModelOpenGLData& instance()
    {
        static CVImageModelOpenGLData self;
        return self;
    }

    CVImageModelOpenGLData(const CVImageModelOpenGLData&) = delete;
    CVImageModelOpenGLData(CVImageModelOpenGLData&&) = delete;
    CVImageModelOpenGLData& operator= (const CVImageModelOpenGLData&) = delete;
    CVImageModelOpenGLData& operator= (CVImageModelOpenGLData&&) = delete;

    ShaderProgram shdProgram;
    VBO imageVBO;
    VAO imageVAO;
    Texture texture;

private:
    CVImageModelOpenGLData();
    ~CVImageModelOpenGLData() {}

#ifdef NDEBUG
  #ifdef _WIN32
    const std::string vertex_path
        = R"(shaders\cvImageVertex.glsl)";
    const std::string fragment_path
        = R"(shaders\cvImageFragment.glsl)";
  #else
    const std::string vertex_path
        = R"(shaders/cvImageVertex.glsl)";
    const std::string fragment_path
        = R"(shaders/cvImageFragment.glsl)";
  #endif
#else
     const std::string vertex_path
         = R"(D:\Egor\projects\cpp\Graphics_Experiments\Rubiks_Cube\shaders\cvImageVertex.glsl)";
     const std::string fragment_path
         = R"(D:\Egor\projects\cpp\Graphics_Experiments\Rubiks_Cube\shaders\cvImageFragment.glsl)";
#endif
};

class WebcamController
{

};