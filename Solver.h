#pragma once
#include "CubeModel.h"
#include "Rotaters.h"
#include "search.h"
#include <string>
#include <sstream>


class Solver
{
public:
    Solver(CubeModel* cm) : cubeModel(cm) {}

    std::string generateSolution() const;
    void solve();

private:
    std::string toString() const;

    CubeModel* cubeModel;
};

