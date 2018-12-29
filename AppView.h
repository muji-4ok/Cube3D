#pragma once
#include "WindowModel.h"
#include "AppController.h"
#include "InteractiveView.h"
#include "WalkthroughView.h"
#include "InputView.h"


class AppView
{
public:
    AppView();
    ~AppView();

private:
    InteractiveView interactiveView;
    WalkthroughView walkthroughView;
    InputView inputView;
    WindowModel windowModel;
    AppController appController;
};

