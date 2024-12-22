#include "PhysicsServer.h"
#include "Log.h"

void FW::Physics::PhysicsServer::update(float delta) {
    for (int i = 0; i < stepSize; i++) {
        for (auto& solver : solvers) {
            solver.update(delta);
        }
    }
}

void FW::Physics::PhysicsServer::setStepSize(int stepSize) {
    if (stepSize < 0) {
        WARN(
          "FW::Physics::stepSize: step size is less than 0. It must be 1 or "
          "greater. It has been reverted to the default fallback value of 1.");
    }

    this->stepSize = stepSize;
}
