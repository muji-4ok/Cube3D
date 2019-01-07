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

void drawButton(const ButtonModel * buttonModel, const WindowModel * windowModel)
{
    drawRect(&buttonModel->rectModel, windowModel);
    drawText(&buttonModel->textModel, windowModel);
}

void drawTextBox(const TextBoxModel * textBoxModel, const WindowModel * windowModel)
{
    drawRect(&textBoxModel->rectModel, windowModel);

    for (const auto& line : textBoxModel->textLines)
        drawText(&line, windowModel);
}

void drawItemBox(const ItemBoxModel * itemBoxModel, const WindowModel * windowModel)
{
    drawRect(&itemBoxModel->rectModel, windowModel);
    /*
    n - canFit
    n * itemRect.size.x + (n - 1) * itemHorSpace + 2 * horPadding <= rectModel.size.x
    */
    int canFit = (itemBoxModel->rectModel.size.x - 2 * itemBoxModel->horPadding + itemBoxModel->itemHorSpace)
               / (itemBoxModel->itemRect.size.x + itemBoxModel->itemHorSpace);

    if (itemBoxModel->items.size() > canFit)
        --canFit;

    float x = itemBoxModel->rectModel.position.x + itemBoxModel->itemHorSpace;
    int i = 0;

    for (auto it = itemBoxModel->items.begin(); it != itemBoxModel->items.end() && i < canFit; ++it, ++i)
    {
        auto textModel = it->first;
        auto rectModel = it->second;
        textModel.position.x += x;
        rectModel.position.x += x;
        
        drawRect(&rectModel, windowModel);
        drawText(&textModel, windowModel);

        x += itemBoxModel->itemRect.size.x + itemBoxModel->itemHorSpace;
    }

    if (itemBoxModel->items.size() > canFit)
    {
        auto textModel = itemBoxModel->placeHolderItemText;
        auto rectModel = itemBoxModel->itemRect;
        textModel.position.x += x;
        rectModel.position.x += x;
        
        drawRect(&rectModel, windowModel);
        drawText(&textModel, windowModel);
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

    float x = textModel->position.x >= 0 ? textModel->position.x : windowModel->width + textModel->position.x;
    float y = textModel->position.y >= 0 ? textModel->position.y : windowModel->height + textModel->position.y;

    for (const auto& c : textModel->text)
    {
        auto& character = textData.characters[c];

        float x_pos = x + character.bearing.x * textModel->scale;
        float y_pos = y - (character.size.y - character.bearing.y) * textModel->scale;

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

    float x = rectModel->position.x >= 0 ? rectModel->position.x : windowModel->width + rectModel->position.x;
    float y = rectModel->position.y >= 0 ? rectModel->position.y : windowModel->height + rectModel->position.y;
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
