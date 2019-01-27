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

std::string SetHitPosRotationHeader::get_name()
{
    return "SetHitPosRotationHeader";
}

std::string SetHitDirRotationHeader::get_name()
{
    return "SetHitDirRotationHeader";
}

const HitHeader& HitModel::get_header() const
{
    return hit;
}

HitHeader&& HitModel::pop_header()
{
    has_dir = false;
    has_position = false;
    return std::move(hit);
}

void HitModel::set_header(const HitHeader & hit)
{
    has_dir = true;
    has_position = true;
    this->hit = hit;
}

void HitModel::set_position(const HitHeader & hit)
{
    has_position = true;
    this->hit.i = hit.i;
    this->hit.j = hit.j;
    this->hit.k = hit.k;
    this->hit.index = hit.index;
}

void HitModel::set_dir(const HitHeader & hit)
{
    has_dir = true;
    this->hit.dir_vec = hit.dir_vec;
    this->hit.dir = hit.dir;
}

void Observable::add_observer(Observer * observer)
{
    observers.push_back(observer);
}

void Observable::notify()
{
    for (auto &o : observers)
        o->update();
}

std::string WholeRotationHeader::get_name()
{
    return "WholeRotationHeader";
}
