#ifndef VERTEXBUFFERTEMPLATE_H_INCLUDED
#define VERTEXBUFFERTEMPLATE_H_INCLUDED

#pragma once

#include <vector>
#include <cstdint>
#include "base/glad/glad.h"

struct VertexAttribute {

    uint8_t size;
    GLenum type;
    bool normalized;

    constexpr VertexAttribute(uint8_t size, GLenum type, bool normalized) : size(size), type(type), normalized(normalized) {}

    void bind(uint32_t idx, uint32_t offset, uint32_t totalSize) const {
        glVertexAttribPointer(idx, size, type, normalized ? GL_TRUE : GL_FALSE, totalSize * sizeof(float), reinterpret_cast<void*>(static_cast<uintptr_t>(offset * sizeof(float))));
        glEnableVertexAttribArray(idx);
    }

    static const VertexAttribute FLOAT;
    static const VertexAttribute VEC2;
    static const VertexAttribute VEC3;
    static const VertexAttribute VEC4;
    static const VertexAttribute INT;
    static const VertexAttribute IVEC2;
    static const VertexAttribute IVEC3;
    static const VertexAttribute IVEC4;
};

struct VertexBufferTemplate {

    std::vector<VertexAttribute> attributes;
    int totalSize = 0;
    bool instance = false;

    VertexBufferTemplate(bool isInstance = false) : instance(isInstance) {}

    VertexBufferTemplate& addAttribute(const VertexAttribute& attr) {
        attributes.push_back(attr);
        totalSize += attr.size;
        return *this;
    }

    GLuint build(uint32_t idx) const {
        GLuint vbo;
        glGenBuffers(1, &vbo);
        build(idx, vbo);
        return vbo;
    }

    void build(uint32_t idx, GLuint vbo) const {
        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        int offset = 0;
        for (const auto& attr : attributes) {
            attr.bind(idx, offset, totalSize);
            if (instance) {
                glVertexAttribDivisor(idx, 1);
            }
            offset += attr.size;
            ++idx;
        }
        glBindBuffer(GL_ARRAY_BUFFER, 0);
    }

    int getSize() const { return totalSize; }
    int getAttributeCount() const { return static_cast<int>(attributes.size()); }
};

#endif

#include "polyray/VertexBufferTemplate.h"

const VertexAttribute VertexAttribute::FLOAT = VertexAttribute(1, GL_FLOAT, false);
const VertexAttribute VertexAttribute::VEC2 =  VertexAttribute(2, GL_FLOAT, false);
const VertexAttribute VertexAttribute::VEC3 =  VertexAttribute(3, GL_FLOAT, false);
const VertexAttribute VertexAttribute::VEC4 =  VertexAttribute(4, GL_FLOAT, false);
const VertexAttribute VertexAttribute::INT =   VertexAttribute(1, GL_INT, false);
const VertexAttribute VertexAttribute::IVEC2 = VertexAttribute(2, GL_INT, false);
const VertexAttribute VertexAttribute::IVEC3 = VertexAttribute(3, GL_INT, false);
const VertexAttribute VertexAttribute::IVEC4 = VertexAttribute(4, GL_INT, false);
