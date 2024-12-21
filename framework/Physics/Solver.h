/**
 * A solver is a piece of code that computes physics related algorithms.
 * It takes some inputs, and the resulting output is a set of actions
 * applied to various physics component. These can be physics bodies, forces and
 * energy.
 * 
 * This project's solver implementation involves putting together forces to
 * impact bodies. Currently, only gravity is supported.
 * 
 * @file Solver.h
 * @author Khai Duong
 * @date 10th of December 2024
 */

/*
    Change log:
*/

#pragma once

#include "pch.h"
#include "Force.h"

namespace FW::Physics {
class Solver {
public:
    Solver() = default;
    virtual ~Solver() = default;

    void update(float delta);
    void addForce(ref<Force> force) { forces.push_back(force); }

protected:
    std::vector<ref<Force>> forces;
};
} // namespace FW::Physics