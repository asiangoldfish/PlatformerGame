#pragma once

class PhysicsScene;
class AppWidget;

/**
 * The Filesystem is a collection of states and queries. This is useful for
 * transferring data between components that otherwise don't know about each
 * other, like the AppWidget and the PhysicsScene.
 */
class Filesystem {
public:
private:
    friend PhysicsScene;
    friend AppWidget;
    
    /**
     * Test for spawning a cube.
     * If a cube should be spawned on the next frame, this is true.
     */
    bool spawnCube = false;
};