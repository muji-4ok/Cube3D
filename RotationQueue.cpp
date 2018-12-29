#include "RotationQueue.h"

bool RotationQueue::is_rotating() const
{
    return queue.size();
}

RotationHeader* RotationQueue::pop()
{
    auto res = std::move(queue.front());
    auto temp_ptr = dynamic_cast<TempRotationHeader*>(res.get());

    if (temp_ptr)
    {
        last_rotation = *temp_ptr;
        angle += last_rotation.angle;
    }

    queue.pop_front();

    return res.release();
}

void RotationQueue::push(RotationHeader* rh)
{
    queue.emplace_back(rh);
}

void RotationQueue::reset_angle()
{
    angle = 0.0f;
}

float RotationQueue::get_last_angle() const
{
    return last_rotation.angle;
}

float RotationQueue::get_angle() const
{
    return angle;
}

TempRotationHeader RotationQueue::get_last_rotation() const
{
    return last_rotation;
}
