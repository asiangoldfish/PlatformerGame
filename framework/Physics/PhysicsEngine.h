/**
 * A solver is a piece of code that computes physics related algorithms.
 * It takes some inputs, and the resulting output is a set of actions
 * applied to various physics component. These can be physics bodies, forces and
 * energy.
 *
 * This project's solver implementation involves putting together forces to
 * impact bodies. Currently, only gravity is supported.
 *
 * The PhysicsEngine is accessible as a singleton, usingMeyer's singleton
 * implementation (https://www.aristeia.com/Papers/DDJ_Jul_Aug_2004_revised.pdf)
 * . You can access it as follows:
 *
 * PhysicsEngine::getInstance()
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
#include "Memory.h"
#include "PhysicsBody.h"

class PhysicsEngine {
public:
    PhysicsEngine();
    virtual ~PhysicsEngine();

    /**
     * Update the underlying physics engines.
     *
     * This interface should only be called once! It is advise to call this in
     * the primary game loop. The physics world itself is updated in a fixed
     * time interval (default is 1/60 second)
     */
    void update(float delta);
    void addForce(ref<Force> force) { forces.push_back(force); }
    void addPhysicsBody(ref<PhysicsBody2D> body);

public:
    /**
     * Stepsize determines how many times per second the simulation iterates.
     * A value of 0 leads to undefined behaviour.
     */
    float timeStep = 1.0f / 30.0f;

    /**
     * The accumulator is the overspilled time since the last step.
     */
    float accumulator = 0.0f;

protected:
    std::vector<ref<Force>> forces;

    std::vector<ref<PhysicsBody2D>> physicsBodies;
};
