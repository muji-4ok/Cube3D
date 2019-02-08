#include "View.h"

void drawCube(const CubeModel * cubeModel, const WindowModel* windowModel)
{
    glEnable(GL_DEPTH_TEST);
    glDisable(GL_CULL_FACE);
    auto& cubeData = CubeModelOpenGLData::instance();
    cubeData.cubeVAO.bind();
    cubeData.shdProgram.use();

    cubeData.shdProgram.setUniformMatrix4fv("view", cubeModel->view);
    cubeData.shdProgram.setUniformMatrix4fv("projection", windowModel->perspectiveProjection);

    for (int i = 0; i < 3; ++i)
        for (int j = 0; j < 3; ++j)
            for (int k = 0; k < 3; ++k)
            {
                cubeData.shdProgram.setUniform1i("i", i);
                cubeData.shdProgram.setUniform1i("j", j);
                cubeData.shdProgram.setUniform1i("k", k);

                auto model = cubeModel->cubelets[i][j][k].rotation * cubeModel->cubelets[i][j][k].translation;

                cubeData.shdProgram.setUniformMatrix4fv("model", model);
                cubeData.shdProgram.setUniformVector3fv("colors", cubeModel->cubelets[i][j][k].colors);

                glDrawArrays(GL_TRIANGLES, 0, 36);
            }
}

void drawButton(ButtonModel * buttonModel, const WindowModel * windowModel)
{
    auto rectModelScreen = buttonModel->rectModel.NDCtoScreen(windowModel);
    drawRect(&rectModelScreen, windowModel);
    drawTextCentered(&buttonModel->textModel, &rectModelScreen, windowModel);
}

void drawTextBox(TextBoxModel * textBoxModel, const WindowModel * windowModel)
{
    auto rectModelScreen = textBoxModel->rectModel.NDCtoScreen(windowModel);
    drawRect(&rectModelScreen, windowModel);
    drawTextCentered(&textBoxModel->textModel, &rectModelScreen, windowModel);
}

void drawItemBox(const ItemBoxModel * itemBoxModel, const WindowModel * windowModel)
{
    auto rectModelScreen = itemBoxModel->getRectModel().NDCtoScreen(windowModel);
    drawRect(&rectModelScreen, windowModel);
    /*
    n - canFit
    n * itemRect.size.x + (n - 1) * itemHorSpace + 2 * horPadding <= rectModel.size.x
    */
    int canFit = (rectModelScreen.size.x - 2 * itemBoxModel->getHorPadding() + itemBoxModel->getItemHorSpace())
               / (itemBoxModel->getItemRect().size.x + itemBoxModel->getItemHorSpace());

    if (itemBoxModel->getItems().size() > canFit)
        --canFit;

    float x = rectModelScreen.position.x + itemBoxModel->getHorPadding();
    int i = 0;

    for (auto it = itemBoxModel->getItems().begin(); it != itemBoxModel->getItems().end() && i < canFit; ++it, ++i)
    {
        auto textModel = it->first;
        auto rectModel = it->second;

        if (i == 0)
            rectModel.color = itemBoxModel->getIBgColorActive();

        rectModel.position.x = x;
        rectModel.position.y = rectModelScreen.position.y + (rectModelScreen.size.y - rectModel.size.y) / 2;

        drawRect(&rectModel, windowModel);
        drawTextCentered(&textModel, &rectModel, windowModel);

        x += itemBoxModel->getItemRect().size.x + itemBoxModel->getItemHorSpace();
    }

    if (itemBoxModel->getItems().size() > canFit)
    {
        auto textModel = itemBoxModel->getPlaceHolderItemText();
        auto rectModel = itemBoxModel->getItemRect();
        rectModel.position.x = x;
        rectModel.position.y = rectModelScreen.position.y + (rectModelScreen.size.y - rectModel.size.y) / 2;

        drawRect(&rectModel, windowModel);
        drawTextCentered(&textModel, &rectModel, windowModel);
    }
}

void drawText(const TextModel * textModel, const WindowModel * windowModel)
{
    glDisable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    auto& textData = TextModelOpenGLData::instance();
    textData.shdProgram.use();
    textData.shdProgram.setUniformMatrix4fv("projection", windowModel->orthogonalProjection);
    textData.shdProgram.setUniform3f("textColor", textModel->color);
    glActiveTexture(GL_TEXTURE0);
    textData.textVAO.bind();
    textData.textVBO.bind();

    float yOffset = 0.0f;
    float lineHeight = 0.0f;

    for (const auto& c : textModel->getText())
    {
        auto& character = textData.characters[c];

        if (c == '\n')
        {
            yOffset += lineHeight + textModel->lineDelim;
            lineHeight = 0.0f;
            continue;
        }

        lineHeight = std::max(lineHeight, character.size.y * textModel->scale);
    }

    float x = textModel->position.x >= 0 ? textModel->position.x :
                                           windowModel->viewportWidth + textModel->position.x;
    float y = textModel->position.y >= 0 ? textModel->position.y :
                                           windowModel->viewportHeight + textModel->position.y;
    y += yOffset;

    float origX = x;
    float origY = y;

    lineHeight = 0.0f;

    for (const auto& c : textModel->getText())
    {
        auto& character = textData.characters[c];

        if (c == '\n')
        {
            y -= lineHeight + textModel->lineDelim;
            x = origX;
            lineHeight = 0.0f;
            continue;
        }

        float x_pos = x + character.bearing.x * textModel->scale;
        float y_pos = y - (character.size.y - character.bearing.y) * textModel->scale;
        lineHeight = std::max(lineHeight, character.size.y * textModel->scale);

        float w = character.size.x * textModel->scale;
        float h = character.size.y * textModel->scale;

        float vertices[6][4] = {
            {x_pos    , y_pos + h, 0.0f, 0.0f},
            {x_pos    , y_pos    , 0.0f, 1.0f},
            {x_pos + w, y_pos    , 1.0f, 1.0f},

            {x_pos    , y_pos + h, 0.0f, 0.0f},
            {x_pos + w, y_pos    , 1.0f, 1.0f},
            {x_pos + w, y_pos + h, 1.0f, 0.0f}
        };

        character.texture->bind();
        textData.textVAO.bind();
        textData.textVBO.bind();
        glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);

        glDrawArrays(GL_TRIANGLES, 0, 6);

        x += (character.advance >> 6) * textModel->scale;
    }
}

void drawTextCentered(TextModel * textModel, const RectangleModel * rectModel, const WindowModel * windowModel)
{
    // assert(textModel->size.x <= rectModel->size.x);
    // assert(textModel->size.y <= rectModel->size.y);
    textModel->position.x = rectModel->position.x + (rectModel->size.x - textModel->size.x) / 2.0;
    textModel->position.y = rectModel->position.y + (rectModel->size.y - textModel->size.y) / 2.0;
    drawText(textModel, windowModel);
}

void drawRect(const RectangleModel * rectModel, const WindowModel * windowModel)
{
    glDisable(GL_DEPTH_TEST);
    glDisable(GL_CULL_FACE);
    auto& rectangleData = RectangleModelOpenGLData::instance();
    rectangleData.shdProgram.use();
    rectangleData.shdProgram.setUniformMatrix4fv("projection", windowModel->orthogonalProjection);
    rectangleData.shdProgram.setUniform3f("rectangleColor", rectModel->color);
    rectangleData.rectVAO.bind();
    rectangleData.rectVBO.bind();

    float x = rectModel->position.x;
    float y = rectModel->position.y;
    float w = rectModel->size.x;
    float h = rectModel->size.y;

    float vertices[6][2] = {
        {x + w, y    },
        {x    , y    },
        {x    , y + h},

        {x    , y + h},
        {x + w, y + h},
        {x + w, y    }
    };

    rectangleData.rectVAO.bind();
    rectangleData.rectVBO.bind();
    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);

    glDrawArrays(GL_TRIANGLES, 0, 6);
}

void drawCVImage(const WebcamModel* webcamModel, const WindowModel* windowModel)
{
    glDisable(GL_DEPTH_TEST);
    auto& imageData = CVImageModelOpenGLData::instance();
    imageData.shdProgram.use();
    imageData.shdProgram.setUniformMatrix4fv("projection", windowModel->orthogonalProjection);

    glPixelStorei(GL_UNPACK_ALIGNMENT, (webcamModel->mat.step & 3) ? 1 : 4);
    glPixelStorei(GL_UNPACK_ROW_LENGTH, webcamModel->mat.step / webcamModel->mat.elemSize());

    imageData.texture.bind();
    imageData.texture.texImage2D(GL_RGB, webcamModel->mat.cols, webcamModel->mat.rows, GL_BGR, GL_UNSIGNED_BYTE,
                                 webcamModel->mat.ptr());

    glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
    glPixelStorei(GL_UNPACK_ROW_LENGTH, 0);

    glActiveTexture(GL_TEXTURE0);
    imageData.imageVAO.bind();
    imageData.imageVBO.bind();

    float winW = windowModel->viewportWidth;
    float winH = windowModel->viewportHeight;
    float imW = webcamModel->mat.cols;
    float imH = webcamModel->mat.rows;

    float x = webcamModel->frame.x;
    float y = webcamModel->frame.y;
    float w = webcamModel->frame.w;
    float h = webcamModel->frame.h;

    assert(x >= 0.0f);
    assert(y >= 0.0f);

    float vertices[6][4] = {
        {x    , y    , 0.0f, 0.0f},
        {x + w, y    , 1.0f, 0.0f},
        {x + w, y + h, 1.0f, 1.0f},

        {x + w, y + h, 1.0f, 1.0f},
        {x    , y + h, 0.0f, 1.0f},
        {x    , y    , 0.0f, 0.0f},
    };

    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);

    glDrawArrays(GL_TRIANGLES, 0, 6);
}

void drawWebcam(const WebcamModel * webcamModel, const WindowModel * windowModel)
{
    drawCVImage(webcamModel, windowModel);

    for (int i = 0; i < 4; ++i)
    {
        RectangleModel verRectModel(glm::vec2(webcamModel->vertDelims[i], webcamModel->frame.y),
                                    glm::vec3(1.0f, 1.0f, 1.0f),
                                    glm::vec2(webcamModel->lineW, webcamModel->frame.h));
        drawRect(&verRectModel, windowModel);
        RectangleModel horRectModel(glm::vec2(webcamModel->frame.x, webcamModel->horDelims[i]),
                                    glm::vec3(1.0f, 1.0f, 1.0f),
                                    glm::vec2(webcamModel->frame.w, webcamModel->lineW));
        drawRect(&horRectModel, windowModel);
    }

    for (int i = 0; i < 3; ++i)
        for (int j = 0; j < 3; ++j)
        {
            auto& x = webcamModel->drawRegions[i][j].x;
            auto& y = webcamModel->drawRegions[i][j].y;
            auto& w = webcamModel->drawRegions[i][j].w;
            auto& h = webcamModel->drawRegions[i][j].h;
            RectangleModel rectModel(glm::vec2(x, y), glm::vec3(1.0f, 0.0f, 0.0f), glm::vec2(w, h));
            auto color = std::string(1, ColorUtil::toChar(ColorUtil::guessColor(webcamModel->meanColors[i][j])));
            TextModel textModel(color, { x, y }, { 1.0f, 0.0f, 1.0f }, 0.2f);
            drawTextCentered(&textModel, &rectModel, windowModel);
        }
}
