#pragma once
#include "Common.h"
#include "CubeModel.h"


class PermRotater
{
public:
    PermRotater(CubeModel* cm);

    void rotate(PermRotationHeader* rot_header);

private:
    CubeModel* cubeModel;

};

class TempRotater
{
public:
    TempRotater(CubeModel* cm);

    void rotate(const TempRotationHeader* rot_header);

private:
    CubeModel* cubeModel;

};

