/**
 * This is a collection of stats related data structures.
 */

#pragma once

struct VitalStats {
    float health = 10.0f;
    float shield = 0.0f;
};

// TODO incorporate weapons to the combat stats
struct CombatStats {
    float damage = 1.0f;
};