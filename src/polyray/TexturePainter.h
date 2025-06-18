#ifndef TEXTUREPAINTER_H_INCLUDED
#define TEXTUREPAINTER_H_INCLUDED

#pragma once
#include <cstdint>
#include <stdexcept>
#include "polyray/Texture.h"

struct TexturePainter {
private:
    const uint32_t width;
    const uint32_t height;
    uint32_t* textureData;
    uint32_t col = 0xFFFFFFFF;

public:
    TexturePainter(uint32_t width, uint32_t height, uint32_t* textureData) : width(width), height(height), textureData(textureData) {
        if (!textureData) {
            throw std::invalid_argument("Texture data pointer cannot be null.");
        }
    }

    void setColor(uint32_t color) {
        col = color;
    }

    void fillRect(uint32_t x, uint32_t y, uint32_t wid, uint32_t hei) {
        uint32_t ex = x + wid;
        uint32_t ey = y + hei;
        if(ex > width) {
            ex = width;
        }
        if(ey > height) {
            ey = height;
        }

        for(uint32_t py = y; py < ey; ey++) {
            uint32_t* row = textureData + py * width;
            for(uint32_t px = x; px < ex; ex++) {
                row[px] = col;
            }
        }
    }

    TexturePainter& operator+=(const Texture& other) {
        if (width != other.width || height != other.height) {
            throw std::invalid_argument("Texture sizes must match for += operation");
        }
        uint8_t* dst = reinterpret_cast<uint8_t*>(textureData);
        const uint8_t* src = reinterpret_cast<const uint8_t*>(other.textureData);

        for (uint32_t i = 0; i < (width * height) << 2; i += 4) {
            uint16_t sumR = dst[i] + src[i];
            dst[i] = sumR > 0xFF ? 0xFF : sumR;
            uint16_t sumG = dst[i + 1] + src[i + 1];
            dst[i + 1] = sumG > 0xFF ? 0xFF : sumG;
            uint16_t sumB = dst[i + 2] + src[i + 2];
            dst[i + 2] = sumB > 0xFF ? 0xFF : sumB;
        }
        return *this;
    }

    TexturePainter& operator-=(const Texture& other) {
        if (width != other.width || height != other.height) {
            throw std::invalid_argument("Texture sizes must match for += operation");
        }
        uint8_t* dst = reinterpret_cast<uint8_t*>(textureData);
        const uint8_t* src = reinterpret_cast<const uint8_t*>(other.textureData);

        for (uint32_t i = 0; i < (width * height) << 2; i += 4) {
            int16_t diffR = dst[i] - src[i];
            dst[i] = diffR < 0 ? 0 : diffR;
            int16_t diffG = dst[i + 1] - src[i + 1];
            dst[i + 1] = diffR < 0 ? 0 : diffR;
            int16_t diffB = dst[i + 2] - src[i + 2];
            dst[i + 2] = diffB < 0 ? 0 : diffB;
        }
        return *this;
    }

    TexturePainter& operator*=(uint8_t scalar) {
        uint8_t* dst = reinterpret_cast<uint8_t*>(textureData);
        for (uint32_t i = 0; i < (width * height) << 2; i += 4) {
            dst[i] = (dst[i] * scalar) >> 4;
            dst[i + 1] = (dst[i + 1] * scalar) >> 4;
            dst[i + 2] = (dst[i + 2] * scalar) >> 4;
        }
        return *this;
    }
};



#endif
