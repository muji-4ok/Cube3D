#include "AppView.h"


AppView::AppView(int width, int height)
{
    windowModel = std::make_unique<WindowModel>(width, height);
    appController = std::make_unique<AppController>(windowModel.get());
    interactiveView = std::make_unique<InteractiveView>(windowModel.get());
    std::cerr << "Constructed AppView\n";
}

void AppView::run()
{
    while (!windowModel->closed)
    {
        std::cerr << "Frame start\n";

        switch (windowModel->appState)
        {
            case Interactive:
                std::cerr << "Event handling start\n";

                while (!windowModel->event_queue_empty())
                    interactiveView->handle_event(windowModel->pop_event());
                
                std::cerr << "Event handling done\n";
                std::cerr << "Drawing start\n";

                interactiveView->draw();

                std::cerr << "Drawing done\n";
                break;
            case Walkthrough:
                break;
            case Input:
                break;
            default:
                throw std::runtime_error("Incorrect appState");
        }

        windowModel->swap_buffers();
        update_fps();

        std::cerr << "Getting new events start\n";

        appController->get_mouse_move_events();
        windowModel->poll_events();

        std::cerr << "Getting new events done\n";

        std::cerr << "Frame done\n";
    }
}

void AppView::update_fps()
{
    static double time_before = 0.0f;
    static int time_counter = 0;

    double time_now = glfwGetTime();
    double mills_passed = (time_now - time_before) * 1000.0;
    time_before = time_now;

    if (time_counter == 30)
    {
        const std::string title = std::to_string(mills_passed);
        windowModel->set_title(title);
        time_counter = 0;
    }

    ++time_counter;
}
