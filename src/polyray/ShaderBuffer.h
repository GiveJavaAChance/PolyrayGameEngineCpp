#ifndef SHADERBUFFER_H_INCLUDED
#define SHADERBUFFER_H_INCLUDED

#pragma once
#include <cstdint>
#include "base/glad/glad.h"

struct ShaderBuffer {
private:
    GLenum usage;

public:
    GLuint ID;
    GLenum target;


    ShaderBuffer(GLenum target, GLenum usage) : target(target), usage(usage) {
        glGenBuffers(1, &ID);
    }

    ShaderBuffer(GLenum target, GLenum usage, GLuint id) : ID(id), target(target), usage(usage) {}

    void setUsage(GLenum newUsage) {
        usage = newUsage;
    }

    void setSize(GLsizeiptr size) const {
        glBindBuffer(target, ID);
        glBufferData(target, size, nullptr, usage);
    }

    void uploadData(const int32_t* data, GLsizeiptr count) const {
        glBindBuffer(target, ID);
        glBufferData(target, count * sizeof(int32_t), data, usage);
    }

    void uploadData(const int64_t* data, GLsizeiptr count) const {
        glBindBuffer(target, ID);
        glBufferData(target, count * sizeof(int64_t), data, usage);
    }

    void uploadData(const float* data, GLsizeiptr count) const {
        glBindBuffer(target, ID);
        glBufferData(target, count * sizeof(float), data, usage);
    }

    void uploadData(const int8_t* data, GLsizeiptr sizeInBytes) const {
        glBindBuffer(target, ID);
        glBufferData(target, sizeInBytes, data, usage);
    }

    void uploadPartialData(const int32_t* data, GLsizeiptr count, GLintptr offset) const {
        glBindBuffer(target, ID);
        glBufferSubData(target, offset, count * sizeof(int32_t), data);
    }

    void uploadPartialData(const int64_t* data, GLsizeiptr count, GLintptr offset) const {
        glBindBuffer(target, ID);
        glBufferSubData(target, offset, count * sizeof(int64_t), data);
    }

    void uploadPartialData(const float* data, GLsizeiptr count, GLintptr offset) const {
        glBindBuffer(target, ID);
        glBufferSubData(target, offset, count * sizeof(float), data);
    }

    void uploadPartialData(const int8_t* data, GLsizeiptr sizeInBytes, GLintptr offset) const {
        glBindBuffer(target, ID);
        glBufferSubData(target, offset, sizeInBytes, data);
    }

    void bind() const {
        glBindBuffer(target, ID);
    }

    void deleteBuffer() {
        glDeleteBuffers(1, &ID);
    }
};

#endif
