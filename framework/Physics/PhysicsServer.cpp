#include "PhysicsServer.h"
#include "assertions.h"

void FW::Physics::PhysicsServer::update(float delta) {
    ASSERT(stepSize > 0, "Step size must be greater than 0.");

    for (int i = 0; i < stepSize; i++) {
        for (auto& solver : solvers) {
            solver.update(delta);
        }
    }
}