#ifndef GLFRAMEBUFFER_H_INCLUDED
#define GLFRAMEBUFFER_H_INCLUDED

#pragma once
#include <stdexcept>
#include "base/glad/glad.h"
#include "polyray/GLTexture.h"

struct GLFramebuffer {
    GLuint ID;
    GLTexture render;
    GLTexture depthTexture;

    GLFramebuffer(uint32_t width, uint32_t height, GLenum colorInterFormat, GLenum colorFormat, GLenum depthInterFormat, GLenum depthFormat) : render(width, height, colorInterFormat, colorFormat, false, false), depthTexture(width, height, depthInterFormat, depthFormat, true, false) {
        glGenFramebuffers(1, &ID);

        bind();

        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, render.ID, 0);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthTexture.ID, 0);

        if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
            throw std::runtime_error("Framebuffer not complete!");
        }
        unbind();
    }

    GLFramebuffer(uint32_t width, uint32_t height) : GLFramebuffer(width, height, GL_RGBA8, GL_RGBA, GL_DEPTH_COMPONENT32, GL_DEPTH_COMPONENT) {}

    void bind() const {
        glBindFramebuffer(GL_FRAMEBUFFER, ID);
    }

    void unbind() const {
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }

    void deleteFramebuffer() {
        glDeleteFramebuffers(1, &ID);
        render.deleteTexture();
        depthTexture.deleteTexture();
    }

    ~GLFramebuffer() {
        deleteFramebuffer();
    }
};

#endif
