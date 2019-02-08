#include "RectangleModel.h"


RectangleModelOpenGLData::RectangleModelOpenGLData()
{
    Shader vertex(GL_VERTEX_SHADER, vertex_path);
    Shader fragment(GL_FRAGMENT_SHADER, fragment_path);
    shdProgram = std::move(ShaderProgram(vertex, fragment));

    rectVBO.generate();
    rectVAO.generate();

    rectVAO.bind();
    rectVBO.bind();

    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 6 * 2, nullptr, GL_DYNAMIC_DRAW);
    rectVAO.setAttribPointer(0, 2, false, 2, 0);
    rectVAO.enableAttribute(0);

    VAO::unbind();
}

RectangleModel RectangleModel::NDCtoScreen(const WindowModel* windowModel) const
{
    return {
        glm::vec2((position.x < 0 ? position.x + 1.0f : position.x) * windowModel->viewportWidth,
                  (position.y < 0 ? position.y + 1.0f : position.y) * windowModel->viewportHeight),
        color,
        glm::vec2(size.x * windowModel->viewportWidth, size.y * windowModel->viewportHeight)
    };
}
