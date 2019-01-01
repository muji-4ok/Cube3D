#include "CubeModel.h"


CubeModel::CubeModel()
{
    for (int i = 0; i < 3; ++i)
        for (int j = 0; j < 3; ++j)
            for (int k = 0; k < 3; ++k)
                cubelets[i][j][k] = { i, j, k };
}
