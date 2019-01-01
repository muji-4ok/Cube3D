#pragma once
#include "WindowModel.h"
#include "AppController.h"
#include "InteractiveView.h"
#include <memory>


class AppView
{
public:
    AppView(int width, int height);

    void run();
    void update_fps();

private:
    std::unique_ptr<InteractiveView> interactiveView;
    // std::unique_ptr<WalkthroughView> walkthroughView;
    // std::unique_ptr<InputView> inputView;

    std::unique_ptr<AppController> appController;
    std::unique_ptr<WindowModel> windowModel;
};

