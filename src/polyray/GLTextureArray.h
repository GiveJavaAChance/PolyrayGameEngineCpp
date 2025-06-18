#ifndef GLTEXTUREARRAY_H_INCLUDED
#define GLTEXTUREARRAY_H_INCLUDED

#pragma once
#include <GLFW/glfw3.h>
#include <stdexcept>
#include <algorithm>
#include <cstdint>
#include "polyray/Texture.h"
#include "polyray/GLTexture.h"

class GLTextureArray : public GLTexture {
    int layers = 0;

public:
    GLTextureArray(uint32_t width, uint32_t height, uint32_t layers, int interFormat, bool interpolate, bool wrap)
        : layers(layers) {
        this->width = width;
        this->height = height;
        this->interFormat = interFormat;
        this->interpolationMode = interpolate ? GL_LINEAR : GL_NEAREST;
        this->wrapMode = wrap ? GL_REPEAT : GL_CLAMP_TO_EDGE;

        glGenTextures(1, &ID);
        glBindTexture(GL_TEXTURE_2D_ARRAY, ID);
        glTexStorage3D(GL_TEXTURE_2D_ARRAY, 1, interFormat, width, height, layers);

        glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MIN_FILTER, interpolationMode);
        glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MAG_FILTER, interpolationMode);
        glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_S, wrapMode);
        glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_T, wrapMode);
        glBindTexture(GL_TEXTURE_2D_ARRAY, 0);
    }

    void setTextureData(Texture& texture) override {
        setTextureData(texture, 0);
    }

    void setTextureData(Texture& texture, uint32_t layer) {
        if (layer >= layers) {
            throw std::out_of_range("Layer index out of bounds.");
        }

        glBindTexture(GL_TEXTURE_2D_ARRAY, ID);
        glTexSubImage3D(GL_TEXTURE_2D_ARRAY, 0, 0, 0, layer, width, height, 1, GL_RGBA, GL_UNSIGNED_BYTE, texture.textureData);
    }

    void setTextureData(Texture& texture, int x, int y) override {
        throw std::runtime_error("setTextureData(x, y) not supported for GLTextureArray");
    }

    void resize(uint32_t, uint32_t) override {
        throw std::runtime_error("resize not supported for GLTextureArray");
    }

    void bind() const override {
        glBindTexture(GL_TEXTURE_2D_ARRAY, ID);
    }
};

#endif
