#ifndef BINDINGREGISTRY_H_INCLUDED
#define BINDINGREGISTRY_H_INCLUDED

#pragma once
#include <cstdint>
#include "base/glad/glad.h"

struct GLTexture;

struct ShaderBuffer;

namespace BindingRegistry {
    uint32_t bindImageTexture(GLTexture& texture, uint32_t level,  bool layered, uint32_t layer, GLenum access, GLenum format);
    uint32_t bindImageTexture(GLTexture& texture, uint32_t level, bool layered, uint32_t layer, GLenum access);
    uint32_t bindBufferBase(ShaderBuffer& buffer, GLenum target);
    uint32_t bindBufferBase(ShaderBuffer& buffer);

    void unbindImageTexture(GLTexture& texture);
    void unbindImageTexture(ShaderBuffer& buffer);
}

#endif
