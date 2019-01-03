#pragma once
#include "CubeModel.h"


class CubeView
{
public:
    CubeView(CubeModel* cm) : cubeModel(cm) {}

    void draw() const;

private:
    CubeModel* cubeModel;

};

