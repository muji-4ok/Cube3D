#include "RotationQueue.h"

RotationQueue::RotationQueue()
{
}

bool RotationQueue::is_rotating()
{
    return queue.size();
}

std::unique_ptr<RotationHeader> RotationQueue::pop()
{
    auto res = std::move(queue.front());
    auto temp_ptr = dynamic_cast<TempRotationHeader*>(res.get());

    if (temp_ptr)
    {
        last_rotation = *temp_ptr;
        angle += last_rotation.angle;
    }

    queue.pop_front();

    return std::move(res);
}

void RotationQueue::push(std::unique_ptr<RotationHeader> rh)
{
    queue.push_back(std::move(rh));
}

HitHeader & RotationQueue::get_hit_header()
{
    return hit_header;
}

void RotationQueue::set_hit_header(HitHeader && hh)
{
    hit_header = std::move(hh);
    is_with_header = true;
}

void RotationQueue::set_dir(Rotation_Dir dir, glm::vec2 dir_vec)
{
    hit_header.dir = dir;
    hit_header.dir_vec = dir_vec;
    is_with_dir = true;
}

void RotationQueue::reset()
{
    angle = 0.0f;
    is_with_dir = false;
    is_with_header = false;
}

bool RotationQueue::has_dir()
{
    return is_with_dir;
}

bool RotationQueue::has_header()
{
    return is_with_header;
}

float RotationQueue::get_last_angle()
{
    return last_rotation.angle;
}

float RotationQueue::get_angle()
{
    return angle;
}

TempRotationHeader RotationQueue::get_last_rotation()
{
    return last_rotation;
}
