#pragma once
#include <vector>


struct View
{
    virtual void update() = 0;
};

struct Model
{
    void addView(View* v)
    {
        views.push_back(v);
    }

    void notify()
    {
        for (auto& v : views)
            v->update();
    }

private:
    std::vector<View*> views;
};

struct Controller
{

};

