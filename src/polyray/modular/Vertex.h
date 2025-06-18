#ifndef VERTEX_H_INCLUDED
#define VERTEX_H_INCLUDED

#pragma once
#include "base/FloatBuffer.h"

class Vertex {
public:
    virtual ~Vertex() = default;
    virtual void put(FloatBuffer& buffer) const = 0;
    virtual bool operator==(const Vertex& other) const = 0;
};

#endif
