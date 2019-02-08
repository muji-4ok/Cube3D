#pragma once
#include "CubeModel.h"
#include "Rotaters.h"
#include "InteractiveInterface.h"
#include "search.h"

extern "C" {
#include "cubedefs.h"
}

#include <string>
#include <sstream>
#include <iterator>


/*
* Inherit from this class to create a new cube solver.
*/
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

class OptimalSolverInitializer
{
public:
    static OptimalSolverInitializer& instance()
    {
        static OptimalSolverInitializer self;
        return self;
    }

    OptimalSolverInitializer(const OptimalSolverInitializer&) = delete;
    OptimalSolverInitializer(OptimalSolverInitializer&&) = delete;
    OptimalSolverInitializer& operator= (const OptimalSolverInitializer&) = delete;
    OptimalSolverInitializer& operator= (OptimalSolverInitializer&&) = delete;

private:
    OptimalSolverInitializer()
    {
        init();
    }
    ~OptimalSolverInitializer() {}
};

class OptimalSolver : public Solver
{
public:
    using Solver::Solver;
    std::string generateSolution() const override;

private:
    std::string toString() const;
};

std::vector<std::string> splitWords(const std::string& s);
