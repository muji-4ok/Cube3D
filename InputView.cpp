#include "InputView.h"


InputView::InputView(WindowModel * wm) : windowModel(wm)
{
    cubeModel = std::make_unique<CubeModel>();
}
