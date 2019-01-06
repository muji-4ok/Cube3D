#include "CubeModel.h"


CubeModel::CubeModel(const WindowModel* wm) : windowModel(wm)
{
    translation_view = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -3.0f));
    view = translation_view;
    rotation_view = glm::mat4(1.0f);

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
