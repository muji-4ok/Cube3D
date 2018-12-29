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
        auto set_hit_ptr = dynamic_cast<SetHitRotationHeader*>(rot_header.get());

        if (temp_ptr)
        {
            // std::cout << "angle: " << temp_ptr->angle << '\n';
            auto hit = rotationQueue.get_hit_header();
            //std::cout << "Hit header: i = " << hit.i
                 //<< " ; j = " << hit.j
                 //<< " ; k = " << hit.k
                 //<< " ; index = " << hit.index << '\n';
            model->rotate_facelet(rotationQueue.get_hit_header(), temp_ptr);
        }

        if (perm_ptr)
        {
            model->rotate_permanent(perm_ptr);
            rotationQueue.reset();
            model->reset_models_rotations();
        }

        if (set_hit_ptr)
        {
            rotationQueue.set_hit_header(std::move(set_hit_ptr->hit));
            //std::cout << "hit header changed:\n";
            auto hit = rotationQueue.get_hit_header();
            //std::cout << "Hit header: i = " << hit.i
                 //<< " ; j = " << hit.j
                 //<< " ; k = " << hit.k
                 //<< " ; index = " << hit.index << '\n';
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

void CubeRenderer::rotate_by_notation(char r)
{
    //std::cout << "r: " << r << '\n';
    auto hit = model->notation_to_hit_header(r);

    //std::cout << "Hit header: i = " << hit.i
         //<< " ; j = " << hit.j
         //<< " ; k = " << hit.k
         //<< " ; index = " << hit.index
         //<< " ; dir = " << hit.dir
         //<< " ; dir_vec: " << hit.dir_vec.x << ' ' << hit.dir_vec.y << '\n';

    bool clockwise = (r < 'Z');

    switch (r)
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

    auto rotations = model->get_rotations_for_script(hit, clockwise, r);
    rotationQueue.push(std::make_unique<SetHitRotationHeader>(hit));

    for (auto &r : rotations)
        rotationQueue.push(std::move(r));
}

void CubeRenderer::rotate_sequence(std::string path)
{
    for (const auto &c : path)
        rotate_by_notation(c);
}
