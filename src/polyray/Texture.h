#ifndef TEXTURE_H_INCLUDED
#define TEXTURE_H_INCLUDED

#pragma once
#include <cstdint>
#include <stdexcept>

struct TexturePainter;

struct Texture {
public:
    const uint32_t width;
    const uint32_t height;
    uint32_t* textureData = nullptr;

    Texture(uint32_t width, uint32_t height, uint32_t* textureData) : width(width), height(height), textureData(textureData) {
        if (!textureData) {
            throw std::invalid_argument("Texture data pointer cannot be null.");
        }
    }

    Texture(uint32_t width, uint32_t height) : width(width), height(height) {
        textureData = new uint32_t[width * height];
    }

    Texture(const Texture& other) : width(other.width), height(other.height) {
        textureData = new uint32_t[width * height];
        std::copy(other.textureData, other.textureData + width * height, textureData);
    }

    Texture(Texture&& other) noexcept : width(other.width), height(other.height), textureData(other.textureData) {
        other.textureData = nullptr;
    }

    ~Texture() {
        delete[] textureData;
    }

    Texture& operator=(const Texture& other) = delete;

    Texture& operator=(Texture&& other) noexcept {
        if (this != &other) {
            delete[] textureData;
            textureData = other.textureData;
            other.textureData = nullptr;
        }
        return *this;
    }

    TexturePainter createGraphics();
};

#endif

#include "polyray/TexturePainter.h"

inline TexturePainter Texture::createGraphics() {
    return TexturePainter(width, height, textureData);
}
