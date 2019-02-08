#pragma once
#include "Common.h"
#include "Events.h"
#include "CubeModel.h"
#include "InteractiveInterface.h"
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
    
protected:
    CubeModel* cubeModel;

};

class InteractiveCubeController : public CubeController
{
public:
    InteractiveCubeController(CubeModel* cm) : CubeController(cm), rotater(cm) {}

    void m_down(const MouseDownEvent* e);
    bool m_up(const MouseUpEvent* e);
    void m_move(const MouseMoveEvent* e);
    int k_pressed(const KeyPressedEvent* e);

    void rotate_script(const KeyPressedEvent e);
    void shuffle();
    void rotate();

private:
    HybridRotater rotater;

};

class InputCubeController : public CubeController
{
public:
    InputCubeController(CubeModel* cm) : CubeController(cm), rotater(cm)
    {
    }

    void rotate();

private:
    HybridRotater rotater;
};