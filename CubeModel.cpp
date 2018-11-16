#include "CubeModel.h"



CubeModel::CubeModel()
{
}


CubeModel::~CubeModel()
{
}

void CubeModel::generate(int width, int height)
{
    set_dimensions(width, height);
    Shader vertex(GL_VERTEX_SHADER, vertex_path);
    Shader fragment(GL_FRAGMENT_SHADER, fragment_path);
    shdProgram = std::move(ShaderProgram(vertex, fragment));

    cubeVBO.generate();
    cubeVAO.generate();

    cubeVBO.bind();
    cubeVBO.setStaticData(raw_vertices);

    cubeVAO.bind();
    cubeVAO.setAttribPointer(0, 4, false, 4, 0);
    cubeVAO.enableAttribute(0);

    cubeVAO.bind();
    cubeVBO.bind();

    shdProgram.use();

    view = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -3.0f));
    // mat = glm::rotate(mat, glm::radians(25.0f), glm::vec3(1.0f, 0.0f, 0.0f));
    // mat = glm::rotate(mat, glm::radians(45.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    projection = glm::perspective(glm::radians(45.0f), static_cast<float>(width) / height, 0.1f, 100.0f);

    shdProgram.setUniformMatrix4fv("view", view);
    shdProgram.setUniformMatrix4fv("projection", projection);
}

void CubeModel::set_dimensions(int width, int height)
{
    this->width = width;
    this->height = height;
    projection = glm::perspective(glm::radians(45.0f), static_cast<float>(width) / height, 0.1f, 100.0f);
    shdProgram.use();
    shdProgram.setUniformMatrix4fv("projection", projection);
}
