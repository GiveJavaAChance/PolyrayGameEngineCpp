#ifndef INSTANCE_H_INCLUDED
#define INSTANCE_H_INCLUDED

#pragma once
#include "base/FloatBuffer.h"

class Instance {
public:
    virtual ~Instance() = default;
    virtual void toFloatBuffer(FloatBuffer& buffer) const = 0;
    virtual bool operator==(const Instance& other) const = 0;
};

#endif
