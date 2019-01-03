#include "CubeModel.h"


CubeModel::CubeModel(int width, int height)
{
    auto& data = CubeModelOpenGLData::instance();
    data.shdProgram.use();

    translation_view = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -3.0f));
    view = translation_view;
    rotation_view = glm::mat4(1.0f);
    projection = glm::perspective(glm::radians(45.0f), static_cast<float>(width) / height, 0.1f, 100.0f);

    data.shdProgram.setUniformMatrix4fv("view", view);
    data.shdProgram.setUniformMatrix4fv("projection", projection);

    for (int i = 0; i < 3; ++i)
        for (int j = 0; j < 3; ++j)
            for (int k = 0; k < 3; ++k)
                cubelets[i][j][k] = Cubelet(i, j, k);
}

void CubeModel::reset_rotations()
{
    for (int i = 0; i < 3; ++i)
        for (int j = 0; j < 3; ++j)
            for (int k = 0; k < 3; ++k)
                cubelets[i][j][k].reset_rotation();
}

CubeModelOpenGLData::CubeModelOpenGLData()
{
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
    VAO::unbind();
}
