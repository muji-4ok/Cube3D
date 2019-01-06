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

