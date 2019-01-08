#include "Webcam.h"


WebcamModel::WebcamModel(const WindowModel* windowModel)
{
    std::string filename(R"(D:\Egor\reaction images\filtered\anime glasses cool.jpg)");
    mat = cv::imread(filename, cv::IMREAD_COLOR);
    cv::flip(mat, mat, 0);
    setRegions(windowModel);
}

void WebcamModel::setDelims(const WindowModel * windowModel)
{
    setFrame(windowModel);

    delim = (std::min(frame.w, frame.h) - 4 * lineW) / 5;
    std::vector<float> delims{ delim, delim * 2, delim * 3, delim * 4};

    float vertPadding = frame.w > frame.h ? (frame.w - frame.h) / 2 : 0;
    float horPadding = frame.h > frame.w ? (frame.h - frame.w) / 2 : 0;

    for (int i = 0; i < 4; ++i)
    {
        vertDelims[i] = frame.x + delims[i] + vertPadding + lineW * i;
        horDelims[i] = frame.y + delims[i] + horPadding + lineW * i;
    }
}

void WebcamModel::setRegions(const WindowModel * windowModel)
{
    setDelims(windowModel);

    for (int i = 0; i < 3; ++i)
        for (int j = 0; j < 3; ++j)
            readRegions[i][j] = { vertDelims[i] + lineW, horDelims[j] + lineW, delim, delim };
}

void WebcamModel::setFrame(const WindowModel* windowModel)
{
    float winW = windowModel->viewportWidth;
    float winH = windowModel->viewportHeight;
    float imW = mat.cols;
    float imH = mat.rows;

    float x, y, w, h;

    w = winW;
    h = imH * (w / imW);

    x = (winW - w) / 2.0f;
    y = (winH - h) / 2.0f;

    if (x < 0.0f)
    {
        w = winW;
        h = imH * (w / imW);
        x = 0.0f;
        y = (winH - h) / 2.0f;
    }
    else if (y < 0.0f)
    {
        h = winH;
        w = imW * (h / imH);
        x = (winW - w) / 2.0f;
        y = 0.0f;
    }

    frame.x = x;
    frame.y = y;
    frame.w = w;
    frame.h = h;
}

CVImageModelOpenGLData::CVImageModelOpenGLData()
{
    Shader vertex(GL_VERTEX_SHADER, vertex_path);
    Shader fragment(GL_FRAGMENT_SHADER, fragment_path);
    shdProgram = std::move(ShaderProgram(vertex, fragment));

    texture.generate();
    texture.bind();

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    imageVAO.generate();
    imageVBO.generate();

    imageVAO.bind();
    imageVBO.bind();

    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 6 * 4, nullptr, GL_DYNAMIC_DRAW);
    imageVAO.enableAttribute(0);
    imageVAO.setAttribPointer(0, 4, false, 4, 0);

    VAO::unbind();
}
