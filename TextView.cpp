#include "TextView.h"

void TextView::draw() const
{
    glEnable(GL_CULL_FACE);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    auto& textData = TextModelOpenGLData::instance();
    textData.shdProgram.use();
    textData.shdProgram.setUniformMatrix4fv("projection", textModel->projection);
    textData.shdProgram.setUniform3f("textColor", textModel->color);
    glActiveTexture(GL_TEXTURE0);
    textData.textVAO.bind();
    textData.textVBO.bind();

    float x = textModel->position.x;
    float y = textModel->position.y;

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

void TextController::d_change(const DimensionsChangeEvent * e)
{
    textModel->set_orthogonal_projection(e->width, e->height);
}
