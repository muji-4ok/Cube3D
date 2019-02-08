#include "CubeController.h"


void InteractiveCubeController::m_down(const MouseDownEvent * e)
{
    if (e->left_pressed && !cubeModel->rotationQueue.is_rotating())
        rotater.set_interactive_pos(e->mouse_pos);
}

bool InteractiveCubeController::m_up(const MouseUpEvent * e)
{
    return e->left_pressed && cubeModel->hitModel.has_position && cubeModel->hitModel.has_dir
        && !cubeModel->rotationQueue.is_rotating();
}

void InteractiveCubeController::m_move(const MouseMoveEvent * e)
{
    if (e->right_pressed && !cubeModel->rotationQueue.is_rotating())
        rotater.rotate_all_interactive(e->mouse_pos);

    if (e->left_pressed && cubeModel->hitModel.has_position && !cubeModel->rotationQueue.is_rotating())
    {
        if (!cubeModel->hitModel.has_dir)
            rotater.set_interactive_dir(e->mouse_pos);
        else
            rotater.rotate_interactive(e->mouse_pos);
    }
}

int InteractiveCubeController::k_pressed(const KeyPressedEvent * e)
{
    if (cubeModel->rotationQueue.is_rotating())
        return 0;

    static std::vector<char> possible{ 'U', 'u', 'D', 'd', 'R', 'r', 'L', 'l', 'F', 'f', 'B', 'b' };

    if (e->key == 'S' || e->key == 's')
        return 1;
    else if (std::find(possible.begin(), possible.end(), e->key) != possible.end())
        return 2;

    return 0;
}

void InteractiveCubeController::rotate_script(const KeyPressedEvent e)
{
    rotater.rotate_script(e.key);
}

void InteractiveCubeController::shuffle()
{
    static std::vector<char> possible{ 'U', 'u', 'D', 'd', 'R', 'r', 'L', 'l', 'F', 'f', 'B', 'b' };

    static std::mt19937 gen(std::time(0));
    static std::uniform_int_distribution<int> dist(0, possible.size() - 1);

    for (int i = 0; i < 30; ++i)
        rotater.rotate_script(possible[dist(gen)]);
}

void InteractiveCubeController::rotate()
{
    rotater.process_queue();
}

void InputCubeController::rotate()
{
    rotater.process_queue();
}
