#include "Common.h"

std::string PermRotationHeader::get_name()
{
    return "PermRotationHeader";
}

std::string TempRotationHeader::get_name()
{
    return "TempRotationHeader";
}

std::string ResetRotationHeader::get_name()
{
    return "ResetRotationHeader";
}

std::string SetHitRotationHeader::get_name()
{
    return "SetHitRotationHeader";
}

HitHeader HitModel::get_header() const
{
    return hit;
}

HitHeader HitModel::pop_header()
{
    has_dir = false;
    has_position = false;
    return hit;
}

void HitModel::set_header(HitHeader && hit)
{
    has_dir = true;
    has_position = true;
    this->hit = std::move(hit);
}

void HitModel::set_position(HitHeader && hit)
{
    has_position = true;
    this->hit = std::move(hit);
}

void HitModel::set_dir(glm::vec2&& dir_vec, RotationDir dir)
{
    has_dir = true;
    hit.dir_vec = dir_vec;
    hit.dir = dir;
}
