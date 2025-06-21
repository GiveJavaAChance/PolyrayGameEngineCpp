#ifndef GLTEXTURE_H_INCLUDED
#define GLTEXTURE_H_INCLUDED

#include <cstdint>
#include "base/glad/glad.h"

struct Texture;

class GLTexture {
protected:
    uint32_t width = 0;
    uint32_t height = 0;
    GLenum interpolationMode = 0;
    GLenum wrapMode = 0;

    GLTexture(uint32_t width, uint32_t height, GLenum interFormat);

public:
    GLuint ID = 0;
    const GLenum interFormat;

    GLTexture() = default;

    GLTexture(uint32_t width, uint32_t height, GLenum interFormat, GLenum format, bool interpolate, bool wrap);

    GLTexture(Texture& texture, GLenum interFormat, bool interpolate, bool wrap);

    virtual void setTextureData(Texture& texture);

    virtual void setTextureData(Texture& texture, int x, int y);

    virtual void resize(uint32_t newWidth, uint32_t newHeight);

    virtual void bind() const;

    void deleteTexture();

    uint32_t getWidth() const;

    uint32_t getHeight() const;

    virtual ~GLTexture() = default;
};

#endif
