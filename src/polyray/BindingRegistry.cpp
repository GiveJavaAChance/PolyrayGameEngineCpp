#include "polyray/BindingRegistry.h"
#include "polyray/GLTexture.h"
#include "polyray/ShaderBuffer.h"
#include "base/glad/glad.h"
#include <cstdint>
#include <unordered_map>
#include <string>
#include <iostream>
#include <algorithm>

namespace {
    std::unordered_map<std::string, uint32_t> usedBindings;

    uint32_t getNextAvailableBinding() {
        uint32_t maxSearchRange = 0;
        for (const auto &kv : usedBindings) {
            if (kv.second > maxSearchRange) {
                maxSearchRange = kv.second;
            }
        }
        maxSearchRange += 2;
        if (maxSearchRange > GL_MAX_IMAGE_UNITS) {
            maxSearchRange = GL_MAX_IMAGE_UNITS;
        }

        for (uint32_t i = 0; i < maxSearchRange; i++) {
            bool isUsed = false;
            for (const auto &kv : usedBindings) {
                if (kv.second == i) {
                    isUsed = true;
                    break;
                }
            }
            if (!isUsed) return i;
        }
        return 0xFFFFFFFFu;
    }
}

namespace BindingRegistry {

    uint32_t bindImageTexture(GLTexture& texture, uint32_t level, bool layered, uint32_t layer, GLenum access, GLenum format) {
        std::string key = "tex" + std::to_string(texture.ID);
        auto it = usedBindings.find(key);
        if (it != usedBindings.end()) {
            return it->second;
        }

        uint32_t binding = getNextAvailableBinding();
        if (binding == 0xFFFFFFFFu) {
            std::cerr << "No available image binding points!" << std::endl;
            return 0xFFFFFFFFu;
        }

        glBindImageTexture(binding, texture.ID, level, layered ? GL_TRUE : GL_FALSE, layer, access, format);
        usedBindings[key] = binding;
        return binding;
    }

    uint32_t bindImageTexture(GLTexture& texture,uint32_t level, bool layered, uint32_t layer, GLenum access) {
        return bindImageTexture(texture, level, layered, layer, access, texture.interFormat);
    }

    uint32_t bindBufferBase(ShaderBuffer& buffer, GLenum target) {
        std::string key = "buf" + std::to_string(buffer.ID);
        auto it = usedBindings.find(key);
        if (it != usedBindings.end()) {
            return it->second;
        }

        uint32_t binding = getNextAvailableBinding();
        if (binding == 0xFFFFFFFFu) {
            std::cerr << "No available image binding points!" << std::endl;
            return 0xFFFFFFFFu;
        }

        glBindBufferBase(target, binding, buffer.ID);
        usedBindings[key] = binding;
        return binding;
    }

    uint32_t bindBufferBase(ShaderBuffer& buffer) {
        return bindBufferBase(buffer, buffer.target);
    }

    void unbindImageTexture(GLTexture& texture) {
        std::string key = "tex" + std::to_string(texture.ID);
        auto it = usedBindings.find(key);
        if (it != usedBindings.end()) {
            glBindImageTexture(it->second, 0, 0, GL_FALSE, 0, GL_READ_WRITE, texture.interFormat);
            usedBindings.erase(it);
        }
    }

    void unbindImageTexture(ShaderBuffer& buffer) {
        std::string key = "buf" + std::to_string(buffer.ID);
        auto it = usedBindings.find(key);
        if (it != usedBindings.end()) {
            glBindBufferBase(buffer.target, it->second, 0);
            usedBindings.erase(it);
        }
    }

}
