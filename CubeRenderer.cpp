#include "CubeRenderer.h"


CubeRenderer::~CubeRenderer()
{
}

bool CubeRenderer::draw()
{
    if (rotationQueue.is_rotating())
    {
        auto rot_header = rotationQueue.pop();
        model->rotate_facelet(rotationQueue.get_hit_header(), rot_header);
    }

    model->draw();

    return true;
}

void CubeRenderer::handle_event(const Event *e)
{
    auto mouse_down = dynamic_cast<const MouseDownEvent*>(e);
    auto mouse_up = dynamic_cast<const MouseUpEvent*>(e);
    auto mouse_move = dynamic_cast<const MouseMoveEvent*>(e);
    auto key_pressed = dynamic_cast<const KeyPressedEvent*>(e);
    auto dimensions_change = dynamic_cast<const DimensionsChangeEvent*>(e);

    if (mouse_down)
    {
        HitHeader hit_header;

        if (model->get_hit_header(mouse_down->mouse_pos, hit_header))
            rotationQueue.set_hit_header(std::move(hit_header));
    }
    else if (mouse_up)
    {

    }
    else if (mouse_move)
    {
        if (mouse_move->right_pressed)
            model->rotate_all_with_mouse(mouse_move->mouse_pos);

        if (mouse_move->left_pressed)
        {
            if (!rotationQueue.has_dir())
            {
                glm::vec2 dir_vec;
                Rotation_Dir dir;
                model->get_dir(rotationQueue.get_hit_header(), mouse_move->mouse_pos, dir_vec, dir);
                rotationQueue.set_dir(dir, dir_vec);
            }

            rotationQueue.push(model->get_rotation_header(mouse_move->mouse_pos, rotationQueue.get_hit_header()));
        }
    }
    else if (key_pressed)
    {

    }
    else if (dimensions_change)
    {
        model->set_dimensions(dimensions_change->width, dimensions_change->height);
    }
}
