#pragma once
#include "Common.h"
#include "Events.h"
#include "CubeModel.h"
#include "CubeHitPositionGetter.h"
#include "Rotaters.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>


class CubeController
{
public:
    CubeController(CubeModel* cm);

    void set_perspective_projection(int width, int height);
    void rotate();

    bool is_rotating() const;
    
protected:
    CubeModel* cubeModel;
    std::unique_ptr<TempRotater> tempRotater;
    std::unique_ptr<PermRotater> permRotater;

};

class InteractiveCubeController : public CubeController
{
public:
    using CubeController::CubeController;

    void m_down(const MouseDownEvent* e);
    void m_up(const MouseUpEvent* e);
    void m_move(const MouseMoveEvent* e);
    void k_pressed(const KeyPressedEvent* e);
    void d_change(const DimensionsChangeEvent* e);

private:
    std::unique_ptr<HybridCubeHitPositionGetter> hit_pos_getter;

};