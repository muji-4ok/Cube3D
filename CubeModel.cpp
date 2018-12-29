#include "CubeModel.h"


CubeModel::CubeModel()
{
    set_solved();
    reset_models_rotations();
    reset_models_translations();
}

void CubeModel::set_solved()
{
    std::vector<float> y{ 1.0f, 1.0f, 0.0f };
    std::vector<float> w{ 1.0f, 1.0f, 1.0f };
    std::vector<float> r{ 1.0f, 0.0f, 0.0f };
    std::vector<float> o{ 1.0f, 0.5f, 0.0f };
    std::vector<float> b{ 0.0f, 0.0f, 1.0f };
    std::vector<float> g{ 0.0f, 1.0f, 0.0f };
    std::vector<float> n{ 0.0f, 0.0f, 0.0f };

    for (int i = 0; i < 3; ++i)
        for (int j = 0; j < 3; ++j)
            for (int k = 0; k < 3; ++k)
            {
                colors[i][j][k].clear();
                append(colors[i][j][k], k == 0 ? y : n);
                append(colors[i][j][k], k == 2 ? w : n);
                append(colors[i][j][k], i == 0 ? r : n);
                append(colors[i][j][k], i == 2 ? o : n);
                append(colors[i][j][k], j == 0 ? b : n);
                append(colors[i][j][k], j == 2 ? g : n);
            }
}

void CubeModel::reset_models_rotations()
{
    for (int i = 0; i < 3; ++i)
        for (int j = 0; j < 3; ++j)
            for (int k = 0; k < 3; ++k)
                models_rotate[i][j][k] = glm::mat4(1.0f);
}

void CubeModel::reset_models_translations()
{
    for (int i = 0; i < 3; ++i)
        for (int j = 0; j < 3; ++j)
            for (int k = 0; k < 3; ++k)
            {
                glm::mat4 model = glm::mat4(1.0f);
                model = glm::translate(model, glm::vec3(0.35f * (i - 1), 0.35f * (j - 1), 0.35f * (k - 1)));

                auto model_full_size = glm::scale(model, glm::vec3(0.35f));
                model = glm::scale(model, glm::vec3(1.0f / 3.0f));

                models_translate[i][j][k] = std::move(model);
                models_translate_full[i][j][k] = std::move(model_full_size);
            }
}
