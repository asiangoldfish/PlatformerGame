#include "Solver.h"

void Solver::update(float delta) {
    glm::vec3 totalAcceleration{ 0.0f };

    for (auto& force : forces) {
        force->update(delta);
    }
}