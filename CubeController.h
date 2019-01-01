#pragma once
#include "Common.h"
#include "Events.h"
#include "CubeModel.h"
#include "Rotaters.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <random>
#include <ctime>


class CubeController
{
public:
    CubeController() {}
    CubeController(CubeModel* cm) : cubeModel(cm) {};

    void set_perspective_projection(int width, int height);

    bool is_rotating() const;
    
protected:
    CubeModel* cubeModel;

};

class InteractiveCubeController : public CubeController
{
public:
    InteractiveCubeController() {}
    InteractiveCubeController(CubeModel* cm) : CubeController(cm)
    {
        rotater = std::make_unique<HybridRotater>(cm);
    }

    void m_down(const MouseDownEvent* e);
    void m_up(const MouseUpEvent* e);
    void m_move(const MouseMoveEvent* e);
    void k_pressed(const KeyPressedEvent* e);
    void d_change(const DimensionsChangeEvent* e);

    void rotate();

private:
    std::unique_ptr<HybridRotater> rotater;
    // std::unique_ptr<Solver> solver;

};

/*
class WalkthroughCubeController : public CubeController
{
public:

private:
    std::unique_ptr<ScriptRotater> rotater;
};
*/

/*
class InputCubeController : public CubeController
{
public:

private:
    std::unique_ptr<WholeRotater> rotater;
};
*/