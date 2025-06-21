#include "GLTexture.h"
#include "polyray/Texture.h"
#include "base/glad/glad.h"
#include <algorithm>

GLTexture::GLTexture(uint32_t width, uint32_t height, GLenum interFormat) : width(width), height(height), interFormat(interFormat) {}

GLTexture::GLTexture(uint32_t width, uint32_t height, GLenum interFormat, GLenum format, bool interpolate, bool wrap) : width(width), height(height), interFormat(interFormat) {
    interpolationMode = interpolate ? GL_LINEAR : GL_NEAREST;
    wrapMode = wrap ? GL_REPEAT : GL_CLAMP_TO_EDGE;

    glGenTextures(1, &ID);
    bind();

    glTexImage2D(GL_TEXTURE_2D, 0, interFormat, width, height, 0, format, GL_UNSIGNED_BYTE, nullptr);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, interpolationMode);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, interpolationMode);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrapMode);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrapMode);
}

GLTexture::GLTexture(Texture& texture, GLenum interFormat, bool interpolate, bool wrap) : width(texture.width), height(texture.height), interFormat(interFormat) {
    interpolationMode = interpolate ? GL_LINEAR : GL_NEAREST;
    wrapMode = wrap ? GL_REPEAT : GL_CLAMP_TO_EDGE;

    glGenTextures(1, &ID);
    bind();

    glTexImage2D(GL_TEXTURE_2D, 0, interFormat, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, texture.textureData);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, interpolationMode);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, interpolationMode);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrapMode);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrapMode);
}

void GLTexture::setTextureData(Texture& texture) {
    if (texture.width != width || texture.height != height) {
        width = texture.width;
        height = texture.height;

        glDeleteTextures(1, &ID);
        glGenTextures(1, &ID);
        bind();

        glTexImage2D(GL_TEXTURE_2D, 0, interFormat, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, texture.textureData);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, interpolationMode);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, interpolationMode);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrapMode);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrapMode);
    } else {
        bind();
        glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, width, height, GL_RGBA, GL_UNSIGNED_BYTE, texture.textureData);
    }
}

void GLTexture::setTextureData(Texture& texture, int x, int y) {
    bind();
    glTexSubImage2D(GL_TEXTURE_2D, 0, x, y, texture.width, texture.height, GL_RGBA, GL_UNSIGNED_BYTE, texture.textureData);
}

void GLTexture::resize(uint32_t newWidth, uint32_t newHeight) {
    GLuint fbo = 0;
    glGenFramebuffers(1, &fbo);
    glBindFramebuffer(GL_FRAMEBUFFER, fbo);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, ID, 0);

    GLuint newTextureID = 0;
    glGenTextures(1, &newTextureID);
    glBindTexture(GL_TEXTURE_2D, newTextureID);
    glTexImage2D(GL_TEXTURE_2D, 0, interFormat, newWidth, newHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);

    glBindFramebuffer(GL_READ_FRAMEBUFFER, fbo);
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
    glBindTexture(GL_TEXTURE_2D, newTextureID);
    glCopyTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, 0, 0, std::min(width, newWidth), std::min(height, newHeight));

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glDeleteFramebuffers(1, &fbo);
    glDeleteTextures(1, &ID);

    ID = newTextureID;
    width = newWidth;
    height = newHeight;

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, interpolationMode);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, interpolationMode);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrapMode);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrapMode);
}

void GLTexture::bind() const {
    glBindTexture(GL_TEXTURE_2D, ID);
}

void GLTexture::deleteTexture() {
    glDeleteTextures(1, &ID);
}

uint32_t GLTexture::getWidth() const {
    return width;
}

uint32_t GLTexture::getHeight() const {
    return height;
}
