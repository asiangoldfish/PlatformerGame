/**
 * Asset.h is a collection of datastructures related to a single asset. Such
 * asset could be an object, entity, or script. They are also known as a
 * blueprint or a prefab.
 */

#pragma once

#include "Image.h"
#include "Framework.h"

/**
 * Base class for assets.
 *
 * An asset is an object that primarily exists in the host system's filesystem,
 * and is loaded into the Editor or the game. The base class handles fundamental
 * features, like IO and memory management, while derivatives tailor
 * implementations to their own needs.
 * 
 * Under the hood, the Asset is a wrapper for 
 */
class Asset {
public:
    Asset();
    virtual ~Asset() = default;

    void setName(std::string name) {
        if (!name.empty()) {
            this->name = name;
        }
    }

private:
    std::string name;
    std::string id; // Usually represented by filepath
    FW::ref<Image> thumbnail;
    std::vector<FW::ref<FW::Entity>> entities;
};