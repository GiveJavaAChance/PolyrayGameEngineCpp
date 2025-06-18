#ifndef RENDEROBJECTBASE_H_INCLUDED
#define RENDEROBJECTBASE_H_INCLUDED

#pragma once
#include <vector>
#include <cstdint>
#include <memory>
#include "polyray/modular/Renderable.h"
#include "polyray/VertexBufferTemplate.h"
#include "polyray/ShaderProgram.h"
#include "polyray/ShaderBuffer.h"
#include "polyray/GLTexture.h"
#include "polyray/modular/Vertex.h"
#include "polyray/modular/Instance.h"

class RenderObjectBase : public Renderable {
protected:
    bool doRender = true;
    bool removed = false;

    std::shared_ptr<ShaderProgram> shader;
    std::vector<const Vertex*> vertices;
    std::vector<const Instance*> instances;
    uint32_t numVertices = 0;
    uint32_t numInstances = 0;

    GLuint vao = 0;
    std::unique_ptr<ShaderBuffer> vbo;
    std::unique_ptr<ShaderBuffer> instanceVbo;
    uint32_t vboLength = 0;
    uint32_t instanceLength = 0;

    std::shared_ptr<GLTexture> texture;
    GLenum mode = GL_TRIANGLES;

public:
    RenderObjectBase(std::shared_ptr<ShaderProgram> shader, std::unique_ptr<ShaderBuffer> vbo, std::unique_ptr<ShaderBuffer> instanceVbo) : shader(std::move(shader)), vbo(std::move(vbo)), instanceVbo(std::move(instanceVbo)) {}

    RenderObjectBase(std::shared_ptr<ShaderProgram> shader, const VertexBufferTemplate& vboTemplate, const VertexBufferTemplate& instanceTemplate) : shader(std::move(shader)) {
        glGenVertexArrays(1, &vao);
        glBindVertexArray(vao);

        vbo = std::make_unique<ShaderBuffer>(GL_ARRAY_BUFFER, GL_DYNAMIC_DRAW, vboTemplate.build(0));
        vboLength = vboTemplate.getSize();

        instanceVbo = std::make_unique<ShaderBuffer>(GL_ARRAY_BUFFER, GL_DYNAMIC_DRAW, instanceTemplate.build(vboTemplate.getAttributeCount()));
        instanceLength = instanceTemplate.getSize();

        glBindVertexArray(0);
    }

    RenderObjectBase(std::shared_ptr<GLTexture> texture, std::shared_ptr<ShaderProgram> shader, const VertexBufferTemplate& vboTemplate, const VertexBufferTemplate& instanceTemplate) : RenderObjectBase(shader, vboTemplate, instanceTemplate) {
        this->texture = texture;
    }

    virtual ~RenderObjectBase() {
        remove();
    }

    void setRenderMode(GLenum mode) {
        this->mode = mode;
    }

    std::shared_ptr<GLTexture> getTexture() const {
        return texture;
    }

    std::shared_ptr<ShaderProgram> getShader() const {
        return shader;
    }

    void addVertex(const Vertex& v) {
        vertices.push_back(&v);
    }

    void addTriangle(const Vertex& a, const Vertex& b, const Vertex& c) {
        vertices.push_back(&a);
        vertices.push_back(&b);
        vertices.push_back(&c);
    }

    void addInstance(const Instance& i) {
        instances.push_back(&i);
    }

    void removeVertex(const Vertex& v) {
        vertices.erase(std::remove(vertices.begin(), vertices.end(), &v), vertices.end());
    }

    void removeInstance(const Instance& i) {
        instances.erase(std::remove(instances.begin(), instances.end(), &i), instances.end());
    }

    void upload() {
        FloatBuffer buffer(vertices.size() * vboLength);
        for (const Vertex* v : vertices) {
            v->put(buffer);
        }

        vbo->bind();
        vbo->uploadData(buffer.buffer, buffer.getLength());
        numVertices = vertices.size();
    }

    void uploadInstances() {
        FloatBuffer buffer(instances.size() * instanceLength);
        for (const Instance* i : instances) {
            i->toFloatBuffer(buffer);
        }

        instanceVbo->bind();
        instanceVbo->uploadData(buffer.buffer, buffer.getLength());
        numInstances = instances.size();
    }

    void clearCPUMemory() {
        vertices.clear();
        instances.clear();
    }

    bool isRemoved() const {
        return removed;
    }

    bool isClear() const {
        return numVertices == 0 || numInstances == 0;
    }

    void remove() {
        if (removed) {
                return;
        }
        if (vbo) {
            vbo->deleteBuffer();
        }
        if (instanceVbo) {
            instanceVbo->deleteBuffer();
        }
        if (vao) {
            glDeleteVertexArrays(1, &vao);
        }
        removed = true;
    }

    virtual void render() = 0;
};

#endif
