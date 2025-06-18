#ifndef SHADERPROGRAM_H_INCLUDED
#define SHADERPROGRAM_H_INCLUDED

#include <iostream>
#include <string>
#include <GL/gl.h>

struct ShaderProgram {

    ShaderProgram(const ShaderProgram&) = delete;
    ShaderProgram& operator=(const ShaderProgram&) = delete;

    ShaderProgram(ShaderProgram&&) = delete;
    ShaderProgram& operator=(ShaderProgram&&) = delete;

    ShaderProgram(const std::string& vertSource, const std::string& fragSource, const std::string& name, GLuint index) : compute(false) {
        program = glCreateProgram();

        vs = glCreateShader(GL_VERTEX_SHADER);
        const char* vertSrcPtr = vertSource.c_str();
        glShaderSource(vs, 1, &vertSrcPtr, nullptr);
        glCompileShader(vs);
        if (!checkCompileStatus(vs, "Vertex Shader")) {
            std::exit(EXIT_FAILURE);
        }

        fs = glCreateShader(GL_FRAGMENT_SHADER);
        const char* fragSrcPtr = fragSource.c_str();
        glShaderSource(fs, 1, &fragSrcPtr, nullptr);
        glCompileShader(fs);
        if (!checkCompileStatus(fs, "Fragment Shader")) {
            std::exit(EXIT_FAILURE);
        }

        glAttachShader(program, vs);
        glAttachShader(program, fs);

        glBindAttribLocation(program, index, name.c_str());

        linkAndValidateProgram();

        glDeleteShader(vs);
        glDeleteShader(fs);
        vs = 0;
        fs = 0;
    }

    ShaderProgram(const std::string& computeSource, const std::string& attribName, GLuint attribIndex) : compute(true) {
        program = glCreateProgram();

        cs = glCreateShader(GL_COMPUTE_SHADER);
        const char* computeSrcPtr = computeSource.c_str();
        glShaderSource(vs, 1, &computeSrcPtr, nullptr);
        glCompileShader(cs);
        if (!checkCompileStatus(cs, "Compute Shader")) {
            std::exit(EXIT_FAILURE);
        }

        glAttachShader(program, cs);

        glBindAttribLocation(program, attribIndex, attribName.c_str());

        linkAndValidateProgram();

        glDeleteShader(cs);
        cs = 0;
    }

    ~ShaderProgram() {
        if (program != 0) {
            glDeleteProgram(program);
            program = 0;
        }
    }

    void use() {
        if (!isOn) {
            glUseProgram(program);
            isOn = true;
        }
    }

    void unuse() {
        if (isOn) {
            glUseProgram(0);
            isOn = false;
        }
    }

    GLuint getProgramId() const {
        return program;
    }

    bool isComputeShader() const {
        return compute;
    }

    void setUniform(const std::string& name, uint32_t count, const uint32_t* values) {
        GLint location = glGetUniformLocation(program, name.c_str());
        if (location == -1) return;
        switch (count) {
            case 1: {
                glUniform1i(location, values[0]);
                break;
            }
            case 2: {
                glUniform2i(location, values[0], values[1]);
                break;
            }
            case 3: {
                glUniform3i(location, values[0], values[1], values[2]);
                break;
            }
            case 4: {
                glUniform4i(location, values[0], values[1], values[2], values[3]);
                break;
            }
            default: {
                break;
            }
        }
    }

    void setUniform(const std::string& name, uint32_t count, const float* values) {
        GLint location = glGetUniformLocation(program, name.c_str());
        if (location == -1) return;

        switch (count) {
            case 1: {
                glUniform1f(location, values[0]);
                break;
            }
            case 2: {
                glUniform2f(location, values[0], values[1]);
                break;
            }
            case 3: {
                glUniform3f(location, values[0], values[1], values[2]);
                break;
            }
            case 4: {
                glUniform4f(location, values[0], values[1], values[2], values[3]);
                break;
            }
            default: {
                break;
            }
        }
    }

private:
    GLuint program;
    GLuint vs = 0, fs = 0, cs = 0;
    const bool compute;
    bool isOn = false;

    bool checkCompileStatus(GLuint shader, const std::string& shaderType) {
        GLint success;
        glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
        if (success != GL_TRUE) {
            char infoLog[512];
            glGetShaderInfoLog(shader, 512, nullptr, infoLog);
            std::cerr << shaderType << " compilation failed:\n" << infoLog << std::endl;
            return false;
        }
        return true;
    }

    void linkAndValidateProgram() {
        glLinkProgram(program);
        GLint linked;
        glGetProgramiv(program, GL_LINK_STATUS, &linked);
        if (linked != GL_TRUE) {
            char infoLog[512];
            glGetProgramInfoLog(program, 512, nullptr, infoLog);
            std::cerr << "Shader program linking failed:\n" << infoLog << std::endl;
            std::exit(EXIT_FAILURE);
        }

        glValidateProgram(program);
        GLint validated;
        glGetProgramiv(program, GL_VALIDATE_STATUS, &validated);
        if (validated != GL_TRUE) {
            char infoLog[512];
            glGetProgramInfoLog(program, 512, nullptr, infoLog);
            std::cerr << "Shader program validation failed:\n" << infoLog << std::endl;
            std::exit(EXIT_FAILURE);
        }
    }
};

#endif
