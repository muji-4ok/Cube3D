#pragma once
#include "CubeModel.h"
#include "RotationQueue.h"
#include "Events.h"
#include "Renderer.h"
#include <glad/glad.h>
#include <glfw/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <random>
#include <ctime>

class CubeRenderer : public Renderer
{
public:
    CubeRenderer(CubeModel *cube_p) : model(cube_p) {}
    ~CubeRenderer();

    bool draw() override;
    void handle_event(const Event *e) override;

private:
    void rotate_by_notation(char r);
    void rotate_sequence(std::string path);
    RotationQueue rotationQueue;
    CubeModel *model = nullptr;
};

