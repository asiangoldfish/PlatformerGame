#pragma once

#include "Framework.h"

class WorldGrid
{
public:
    WorldGrid();
    void draw(const FW::ref<FW::Shader> shader);

private:
    FW::ref<FW::VertexArray> vertexArray;
    FW::ref<FW::VertexBuffer> vertexBuffer;
    FW::ref<FW::IndexBuffer> indexBuffer;
};
