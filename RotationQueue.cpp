#include "RotationQueue.h"

RotationQueue::RotationQueue()
{
}

bool RotationQueue::is_rotating()
{
    return queue.size();
}

RotationHeader && RotationQueue::pop()
{
    RotationHeader&& res = std::move(queue.front());
    queue.pop();

    return std::move(res);
}

void RotationQueue::push(RotationHeader && rh)
{
    queue.push(std::move(rh));
}

HitHeader & RotationQueue::get_hit_header()
{
    return hit_header;
}

void RotationQueue::set_hit_header(HitHeader && hh)
{
    hit_header = std::move(hh);
    is_with_dir = false;
}

void RotationQueue::set_dir(Rotation_Dir dir, glm::vec2 dir_vec)
{
    hit_header.dir = dir;
    hit_header.dir_vec = dir_vec;
    is_with_dir = true;
}

bool RotationQueue::has_dir()
{
    return is_with_dir;
}
