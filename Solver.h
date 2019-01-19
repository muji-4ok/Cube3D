#pragma once
#include "CubeModel.h"
#include "Rotaters.h"
#include "search.h"

extern "C" {
#include "cubedefs.h"
}

#include <string>
#include <sstream>


struct Solver
{
    Solver(const CubeModel* cm) : cubeModel(cm) {}

    virtual std::string generateSolution() const = 0;

protected:
    const CubeModel* cubeModel;
};

class FastSolver : public Solver
{
public:
    using Solver::Solver;
    std::string generateSolution() const override;

private:
    std::string toString() const;
};

class OptimalSolver : public Solver
{
public:
    using Solver::Solver;
    std::string generateSolution() const override;

private:
    std::string toString() const;
};
