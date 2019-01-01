#include "CubeController.h"


void CubeController::set_perspective_projection(int width, int height)
{
    cubeModel->projection = glm::perspective(glm::radians(45.0f), static_cast<float>(width) / height, 0.1f, 100.0f);
}

bool CubeController::is_rotating() const
{
    return cubeModel->rotationQueue.is_rotating();
}

void InteractiveCubeController::m_down(const MouseDownEvent * e)
{
    if (e->left_pressed && !cubeModel->rotationQueue.is_rotating())
        rotater->set_interactive_pos(e->mouse_pos);
}

void InteractiveCubeController::m_up(const MouseUpEvent * e)
{
    if (e->left_pressed && cubeModel->hitModel.has_position && cubeModel->hitModel.has_dir)
        rotater->finish_interactive_rotation();
}

void InteractiveCubeController::m_move(const MouseMoveEvent * e)
{
    if (e->right_pressed)
        rotater->rotate_all_interactive(e->mouse_pos);

    if (e->left_pressed && cubeModel->hitModel.has_position && !cubeModel->rotationQueue.is_rotating())
    {
        if (!cubeModel->hitModel.has_dir)
            rotater->set_interactive_dir(e->mouse_pos);

        rotater->rotate_interactive(e->mouse_pos);
    }
}

void InteractiveCubeController::k_pressed(const KeyPressedEvent * e)
{
    if (cubeModel->rotationQueue.is_rotating())
        return;

    if (e->key == 'S' || e->key == 's')
    {
        static std::mt19937 gen(std::time(0));
        static std::vector<char> possible{ 'U', 'u', 'D', 'd', 'R', 'r', 'L', 'l', 'F', 'f', 'B', 'b' };
        static std::uniform_int_distribution<int> dist(0, possible.size() - 1);

        for (int i = 0; i < 30; ++i)
            rotater->rotate_script(possible[dist(gen)]);
    }
    else if (e->key == 'C' || e->key == 'c')
    {
        // auto sol_str = solver->gen_solution();

        // for (const auto &r : sol_str)
        //     rotater->rotate_script(r);
    }
    else
    {
        rotater->rotate_script(e->key);
    }
}

void InteractiveCubeController::d_change(const DimensionsChangeEvent * e)
{
    set_perspective_projection(e->width, e->height);
}

void InteractiveCubeController::rotate()
{
    rotater->process_queue();
}