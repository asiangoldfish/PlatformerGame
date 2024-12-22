#include "PhysicsServer.h"
#include "Log.h"

void FW::Physics::PhysicsServer::update(float delta) {
    for (int i = 0; i < stepSize; i++) {
        for (auto& solver : solvers) {
            solver.update(delta);
        }
    }
}