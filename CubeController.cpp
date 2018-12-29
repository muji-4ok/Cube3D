#include "CubeController.h"


CubeController::CubeController(CubeModel * cm) : cubeModel(cm)
{
}

void CubeController::set_perspective_projection(int width, int height)
{
    cubeModel->projection = glm::perspective(glm::radians(45.0f), static_cast<float>(width) / height, 0.1f, 100.0f);
}

void CubeController::rotate()
{
    if (cubeModel->rotationQueue.is_rotating())
    {
        auto rot_header = cubeModel->rotationQueue.pop();

        auto temp_ptr = dynamic_cast<TempRotationHeader*>(rot_header);
        auto perm_ptr = dynamic_cast<PermRotationHeader*>(rot_header);
        auto reset_ptr = dynamic_cast<ResetRotationHeader*>(rot_header);
        auto set_hit_ptr = dynamic_cast<SetHitRotationHeader*>(rot_header);

        if (temp_ptr)
            tempRotater->rotate(temp_ptr);
        else if (perm_ptr)
            permRotater->rotate(perm_ptr);
        else if (set_hit_ptr)
            cubeModel->hitModel.set_header(std::move(set_hit_ptr->hit));
        else if (reset_ptr)
            ;
        else
            throw std::runtime_error("Incorrect pointer");
    }
}

bool CubeController::is_rotating() const
{
    return cubeModel->rotationQueue.is_rotating();
}

void InteractiveCubeController::m_down(const MouseDownEvent * e)
{
    if (e->left_pressed && !cubeModel->rotationQueue.is_rotating())
    {
        HitHeader hit_header;

        if (hit_pos_getter->get_i(e->mouse_pos, hit_header))
            cubeModel->hitModel.set_header(std::move(hit_header));
    }
}

void InteractiveCubeController::m_up(const MouseUpEvent * e)
{
    if (e->left_pressed && cubeModel->hitModel.has_position && cubeModel->hitModel.has_dir)
    {
        auto rotations = model->get_rotations_to_orthogonal(rotationQueue.get_angle(),
                                                            rotationQueue.get_hit_header(),
                                                            rotationQueue.get_last_rotation());

        for (auto &r : rotations)
            rotationQueue.push(std::move(r));
    }
}

void InteractiveCubeController::m_move(const MouseMoveEvent * e)
{
    if (e->right_pressed)
    {
        model->rotate_all_with_mouse(mouse_move->mouse_pos);
    }

    if (e->left_pressed && cubeModel->hitModel.has_position && !cubeModel->rotationQueue.is_rotating())
    {
        if (!cubeModel->hitModel.has_dir)
        {
            glm::vec2 dir_vec;
            RotationDir dir;
            model->get_dir(rotationQueue.get_hit_header(), mouse_move->mouse_pos, dir_vec, dir);
            rotationQueue.set_dir(dir, dir_vec);
        }

        rotationQueue.push(std::move(model->get_rotation_header(mouse_move->mouse_pos,
                           rotationQueue.get_hit_header())));
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
            rotate_by_notation(possible[dist(gen)]);

        return;
    }
    else if (key_pressed->key == 'C' || key_pressed->key == 'c')
    {
        auto sol_str = model->gen_solution();
        std::cout << sol_str << '\n';
        rotate_sequence(sol_str);
    }
    else
    {
        rotate_by_notation(key_pressed->key);
    }
}

void InteractiveCubeController::d_change(const DimensionsChangeEvent * e)
{
    set_perspective_projection(e->width, e->height);
}
