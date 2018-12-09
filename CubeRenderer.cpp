#include "CubeRenderer.h"


CubeRenderer::~CubeRenderer()
{
}

bool CubeRenderer::draw()
{
    // std::cerr << "draw start\n";

    if (rotationQueue.is_rotating())
    {
        // std::cerr << "rotation start\n";

        auto rot_header = rotationQueue.pop();

        // std::cerr << "pop done\n";

        auto temp_ptr = dynamic_cast<TempRotationHeader*>(rot_header.get());
        auto perm_ptr = dynamic_cast<PermRotationHeader*>(rot_header.get());
        auto reset_ptr = dynamic_cast<ResetRotationHeader*>(rot_header.get());

        if (temp_ptr)
        {
            // std::cout << "angle: " << temp_ptr->angle << '\n';
            model->rotate_facelet(rotationQueue.get_hit_header(), temp_ptr);
        }

        if (perm_ptr)
        {
            model->rotate_permanent(perm_ptr);
            rotationQueue.reset();
            model->reset_models_rotations();
        }

        if (reset_ptr)
        {
            // std::cout << "queue size: " << rotationQueue.queue.size() << '\n';
        }
    }

    // std::cout << "angle: " << rotationQueue.get_angle() << '\n';

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
        if (mouse_down->left_pressed)
        {
            if (rotationQueue.is_rotating())
                return;

            HitHeader hit_header;

            if (model->get_hit_header(mouse_down->mouse_pos, hit_header))
            {
                // std::cout << "Hit header: i = " << hit_header.i
                    // << " ; j = " << hit_header.j
                    // << " ; k = " << hit_header.k
                    // << " ; index = " << hit_header.index << '\n';
                rotationQueue.set_hit_header(std::move(hit_header));
            }
            else
            {
                // std::cout << "Not hit\n";
            }

            // std::cerr << "down done\n";
        }
    }
    else if (mouse_up)
    {
        if (mouse_up->left_pressed)
        {
            if (!rotationQueue.has_header() || !rotationQueue.has_dir())
                return;

            // std::cout << "angle: " << rotationQueue.get_angle() << '\n';
            auto rotations = model->get_rotations_to_orthogonal(rotationQueue.get_angle(),
                                                                rotationQueue.get_hit_header(),
                                                                rotationQueue.get_last_rotation());

            for (auto &r : rotations)
                rotationQueue.push(std::move(r));
            // model->reset_models_rotations(); // change later
            // rotationQueue.reset();
        }

        // std::cerr << "up done\n";
    }
    else if (mouse_move)
    {
        if (mouse_move->right_pressed)
        {
            model->rotate_all_with_mouse(mouse_move->mouse_pos);
        }

        // std::cout << "is_rotating: " << rotationQueue.is_rotating() << '\n';

        if (mouse_move->left_pressed && rotationQueue.has_header() && !rotationQueue.is_rotating())
        {
            // std::cout << "queue size when moving: " << rotationQueue.queue.size() << '\n';

            if (!rotationQueue.has_dir())
            {
                glm::vec2 dir_vec;
                Rotation_Dir dir;
                model->get_dir(rotationQueue.get_hit_header(), mouse_move->mouse_pos, dir_vec, dir);
                rotationQueue.set_dir(dir, dir_vec);
            }

            rotationQueue.push(std::move(model->get_rotation_header(mouse_move->mouse_pos,
                               rotationQueue.get_hit_header())));
        }

        // std::cerr << "move done\n";
    }
    else if (key_pressed)
    {
        if (rotationQueue.is_rotating())
            return;

        auto hit = model->notation_to_hit_header(key_pressed->key);

        // std::cout << "Hit header: i = " << hit.i
            // << " ; j = " << hit.j
            // << " ; k = " << hit.k
            // << " ; index = " << hit.index << '\n';

        bool clockwise = (key_pressed->key < 'Z');

        switch (key_pressed->key)
        {
            case 'U':
            case 'u':
            case 'R':
            case 'r':
            case 'F':
            case 'f':
                clockwise = !clockwise;
                break;
        }

        auto rotations = model->get_rotations_for_script(hit, clockwise, key_pressed->key);
        rotationQueue.set_hit_header(std::move(hit));

        for (auto &r : rotations)
            rotationQueue.push(std::move(r));
    }
    else if (dimensions_change)
    {
        model->set_dimensions(dimensions_change->width, dimensions_change->height);
    }
}
