/**
 * The physics server is the high-level API that the Engine communicates with.
 * It abstracts the lower level details, so the rest of the code base is not
 * reliant on the underlying physics technology.
 * 
 * Only the Engine itself is supposed to interact with the physics server. If
 * there are missing features, then the physics server should be expanded to
 * cover these features.
 * 
 * @file PhysicsServer.h
 * @author Khai Duong
 * @date 10th of December 2024
 */

#pragma once

#include "pch.h"

#include "Solver.h"

namespace FW::Physics {

class PhysicsServer {
public:
    PhysicsServer() = default;
    virtual ~PhysicsServer() = default;

    /**
     * Update the underlying physics engines.
     */
    void update();

    void setDelta(float delta) {
        this->delta = delta;
    }

public:
    /**
     * Global step size.
     * 
     * NB! This can have a very big performance impact. If you need to update an
     * individual component's step size, consider changing that instead. This
     * global step size will increase the total step size for each component
     * multiplicatively.
     */
    int stepSize = 1;

private:
    std::vector<Solver> solvers;
    float delta = 1.0/60.0;
};

} // namespace FW::Physics