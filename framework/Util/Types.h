/**
 * This file provides a list of macros that shortens abstracts the Framework's
 * internal types. It provides an API-independent type system, so the client
 * does not need to worry about which type the Engine uses. It can also be
 * used by the Framework itself.
 */

#pragma once

#include "glm/glm.hpp"

#define Vector2 glm::vec2
#define Vector3 glm::vec3
#define Matrix4 glm::mat4