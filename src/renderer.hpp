#pragma once

#include "indexBuffer.hpp"
#include "vertexArray.hpp"
#include "shader.hpp"

class Renderer {
public:
    void draw(const VertexArray& va,const IndexBuffer& ib, const Shader& shader) const;
    void clear() const;
};