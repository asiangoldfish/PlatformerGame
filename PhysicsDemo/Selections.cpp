#include "Selections.h"

SelectedNode::SelectedNode(FW::Entity* node) {
    selectedNode = node;
}

SelectedNode SelectedNode::operator=(FW::Entity* const& node) {
    return SelectedNode(node);
}
