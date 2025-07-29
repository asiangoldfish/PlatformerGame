/**
 * Selections.h is a collection of datastructures passed between various
 * objects.
 *
 * None of the datastructures manage life cycles. They only reference objects.
 */
#pragma once

#include "Entity.h"

/**
 * Holds data about a specific node.
 */
class SelectedNode {
public:
    SelectedNode() = default;
    SelectedNode(FW::Entity* node);

    FW::Entity* getSelectedNode() { return selectedNode; }
    void setSelectedNode(FW::Entity* node) { selectedNode = node; }

    SelectedNode operator=(FW::Entity* const& node);

private:
    FW::Entity* selectedNode;
};