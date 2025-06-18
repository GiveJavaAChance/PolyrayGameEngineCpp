#ifndef RENDEROBJECT_H_INCLUDED
#define RENDEROBJECT_H_INCLUDED

#include "polyray/modular/RenderObjectBase.h"

class RenderObject : public RenderObjectBase {
public:
    using RenderObjectBase::RenderObjectBase;

    void render() override {
        if (isRemoved() || isClear()) return;

        shader->use();
        glBindVertexArray(vao);
        texture->bind();

        glDrawArraysInstanced(mode, 0, numVertices, numInstances);

        glBindVertexArray(0);
        shader->unuse();
    }
};


#endif
