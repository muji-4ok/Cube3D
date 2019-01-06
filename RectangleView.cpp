#include "RectangleView.h"


void RectangleView::draw(const glm::mat4& projection) const
{
    glDisable(GL_DEPTH_TEST);
    auto& rectangleData = RectangleModelOpenGLData::instance();
    rectangleData.shdProgram.use();
    rectangleData.shdProgram.setUniformMatrix4fv("projection", projection);
    rectangleData.shdProgram.setUniform3f("rectangleColor", rectangleModel->color);
    rectangleData.rectVAO.bind();
    rectangleData.rectVBO.bind();

    float x = rectangleModel->position.x;
    float y = rectangleModel->position.y;
    float w = rectangleModel->size.x;
    float h = rectangleModel->size.y;

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
