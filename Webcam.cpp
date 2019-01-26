#include "Webcam.h"
#include <iostream>


WebcamModel::WebcamModel(const WindowModel* windowModel)
{
    std::string filename(R"(D:\Egor\random a\MnXhY0aVZbI.jpg)");
    mat = cv::imread(filename, cv::IMREAD_COLOR);
    cv::flip(mat, mat, 0);
}

void WebcamModel::resize(const WindowModel * windowModel)
{
    setFrame(windowModel);
    setDelims();
    setDrawRegions();
    setReadRegions();
}

void WebcamModel::setDelims()
{
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

// Sets regions mapped to coordinates on viewport screen, that will be displayed into
void WebcamModel::setDrawRegions()
{
    for (int i = 0; i < 3; ++i)
        for (int j = 0; j < 3; ++j)
            drawRegions[i][j] = { vertDelims[i] + lineW, horDelims[j] + lineW, delim, delim };
}

// Sets regions mapped to cv::Mat for reading pixel data
void WebcamModel::setReadRegions()
{
    float lineW = mat.cols / frame.w * this->lineW;
    float delim = (std::min(mat.cols, mat.rows) - 4 * lineW) / 5;
    std::vector<float> delims{ delim, delim * 2, delim * 3, delim * 4};
    float vertPadding = mat.cols > mat.rows ? (mat.cols - mat.rows) / 2 : 0;
    float horPadding = mat.rows > mat.cols ? (mat.rows - mat.cols) / 2 : 0;

    for (int i = 0; i < 3; ++i)
        for (int j = 0; j < 3; ++j)
            readRegions[i][j] = { delims[i] + vertPadding + lineW * (i + 1),
                                  delims[j] + horPadding + lineW * (j + 1), delim, delim };
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

void WebcamController::resizeToFit(const WindowModel * windowModel)
{
    webcamModel->resize(windowModel);
}

void WebcamController::readFrame()
{
}

void WebcamController::updateColors()
{
    for (int i = 0; i < 3; ++i)
        for (int j = 0; j < 3; ++j)
        {
            auto& frame = webcamModel->readRegions[i][j];
            cv::Rect rect(frame.x, frame.y, frame.w, frame.h);
            cv::Mat region(webcamModel->mat, rect);
            auto meanColor = cv::mean(region);
            webcamModel->meanColors[i][j] = { meanColor[2], meanColor[1], meanColor[0] };
        }
}

char ColorUtil::toChar(SideColor c)
{
    switch (c)
    {
        case Yellow:
            return 'Y';
        case White:
            return 'W';
        case Red:
            return 'R';
        case Orange:
            return 'O';
        case Blue:
            return 'B';
        case Green:
            return 'G';
        default:
            assert(0);
    }
}

SideColor ColorUtil::toEnum(char c)
{
    switch (c)
    {
        case 'Y':
            return Yellow;
        case 'W':
            return White;
        case 'R':
            return Red;
        case 'O':
            return Orange;
        case 'B':
            return Blue;
        case 'G':
            return Green;
        default:
            assert(0);
    }
}

SideColor ColorUtil::guessColor(const glm::vec3 & colorVec)
{
    auto getHue = [](const glm::vec3& c) -> float {
        const auto& r = c[0];
        const auto& g = c[1];
        const auto& b = c[2];
        const auto& v = std::max({ r, g, b });
        const auto& u = std::min({ r, g, b });

        if (u == v)
            return 180.0f;

        if (v == r)
            return 60.0f * (g - b) / (v - u);
        else if (v == g)
            return 120.0f + 60.0f * (b - r) / (v - u);
        else
            return 240.0f + 60.0f * (r - g) / (v - u);
    };
    static std::map<SideColor, float> colorMap{
        { Yellow, getHue({ 1.0f, 1.0f, 0.0f }) },
        { White, getHue({ 1.0f, 1.0f, 1.0f }) },
        { Red, getHue({ 1.0f, 0.0f, 0.0f }) },
        { Orange, getHue({ 1.0f, 0.5f, 0.0f }) },
        { Blue, getHue({ 0.0f, 0.0f, 1.0f }) },
        { Green, getHue({ 0.0f, 1.0f, 0.0f }) },
    };

    auto colorHue = getHue(colorVec);
    SideColor bestGuess = White;
    float bestDiff = 1e6;

    for (auto it = colorMap.begin(); it != colorMap.end(); ++it)
    {
        auto diff = std::abs(it->second - colorHue);

        if (diff < bestDiff)
        {
            bestDiff = diff;
            bestGuess = it->first;
        }
    }

    return bestGuess;
}

glm::vec3 ColorUtil::normalizedColor(const glm::vec3 & c)
{
    return c / 255.0f;
}
