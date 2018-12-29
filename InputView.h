#pragma once
#include "CubeModel.h"
#include "WindowModel.h"
#include <memory>


class InputView
{
public:
    InputView(WindowModel* wm);

    void draw();

private:
    std::unique_ptr<CubeModel> cubeModel;
    WindowModel* windowModel;

};

