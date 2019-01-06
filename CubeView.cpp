#include "CubeView.h"

void CubeView::draw() const
{
    glEnable(GL_DEPTH_TEST);
    glDisable(GL_CULL_FACE);
    auto& cubeData = CubeModelOpenGLData::instance();
    cubeData.cubeVAO.bind();
    cubeData.shdProgram.use();

    cubeData.shdProgram.setUniformMatrix4fv("view", cubeModel->view);
    cubeData.shdProgram.setUniformMatrix4fv("projection", cubeModel->projection);

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
