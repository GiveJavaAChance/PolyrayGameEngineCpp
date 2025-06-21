#ifndef TRANSFORM3D_H_INCLUDED
#define TRANSFORM3D_H_INCLUDED

#pragma once
#include <cmath>
#include <stdexcept>
#include <cstring>
#include <cstdint>
#include "base/FloatBuffer.h"
#include "polyray/Vector3f.h"

struct Transform3D {
public:
    float* matrix;

    Transform3D() {
        matrix = new float[16];
        setToIdentity();
    }

    Transform3D(float* matrix) : matrix(matrix) {
    }

    ~Transform3D() {
        delete[] matrix;
    }

    void setToIdentity() {
        for (int i = 0; i < 16; i++) {
            matrix[i] = 0.0f;
        }
        matrix[0] = matrix[5] = matrix[10] = matrix[15] = 1.0f;
    }

    void set(const Transform3D& other) {
        std::memcpy(matrix, other.matrix, 16 * sizeof(float));
    }

    void setToAxisTransform(const Vector3f& xAxis, const Vector3f& yAxis, const Vector3f& zAxis, const Vector3f& origin) {
        matrix[0] = xAxis.x;
        matrix[1] = xAxis.y;
        matrix[2] = xAxis.z;
        matrix[3] = 0.0f;
        matrix[4] = yAxis.x;
        matrix[5] = yAxis.y;
        matrix[6] = yAxis.z;
        matrix[7] = 0.0f;
        matrix[8] = zAxis.x;
        matrix[9] = zAxis.y;
        matrix[10] = zAxis.z;
        matrix[11] = 0.0f;
        matrix[12] = origin.x;
        matrix[13] = origin.y;
        matrix[14] = origin.z;
        matrix[15] = 1.0f;
    }

    void translate(float tx, float ty, float tz) {
        Transform3D translation;
        translation.matrix[12] = tx;
        translation.matrix[13] = ty;
        translation.matrix[14] = tz;
        multiply(translation);
    }

    void scale(float sx, float sy, float sz) {
        Transform3D scaling;
        scaling.matrix[0] = sx;
        scaling.matrix[5] = sy;
        scaling.matrix[10] = sz;
        multiply(scaling);
    }

    void rotateX(float angle) {
        float s = sinf(angle);
        float c = cosf(angle);
        Transform3D rotation;
        rotation.matrix[5] = c;
        rotation.matrix[6] = -s;
        rotation.matrix[9] = s;
        rotation.matrix[10] = c;
        multiply(rotation);
    }

    void rotateY(float angle) {
        float s = sinf(angle);
        float c = cosf(angle);
        Transform3D rotation;
        rotation.matrix[0] = c;
        rotation.matrix[2] = s;
        rotation.matrix[8] = -s;
        rotation.matrix[10] = c;
        multiply(rotation);
    }

    void rotateZ(float angle) {
        float s = sinf(angle);
        float c = cosf(angle);
        Transform3D rotation;
        rotation.matrix[0] = c;
        rotation.matrix[1] = -s;
        rotation.matrix[4] = s;
        rotation.matrix[5] = c;
        multiply(rotation);
    }

    void multiply(const Transform3D& other) {
        float* result = new float[16];
        for (int row = 0; row < 4; row++) {
            int ri = row << 2;
            for (int col = 0; col < 4; col++) {
                result[ri + col]
                        = matrix[ri] * other.matrix[col]
                        + matrix[ri + 1] * other.matrix[col + 4]
                        + matrix[ri + 2] * other.matrix[col + 8]
                        + matrix[ri + 3] * other.matrix[col + 12];
            }
        }
        std::memcpy(matrix, result, 16 * sizeof(float));
        delete[] result;
    }

    Transform3D inverse() {
        Transform3D out;
        if (!invertMatrix(matrix, out.matrix)) {
            throw std::runtime_error("Matrix cannot be inverted");
        }
        return out;
    }

    void invert() {
        if (!invertMatrix(matrix, matrix)) {
            throw std::runtime_error("Matrix cannot be inverted");
        }
    }

    Transform3D transposed() {
        Transform3D out(toColumnMajor());
        return out;
    }

    void transpose() {
        float* trans = toColumnMajor();
        std::memcpy(matrix, trans, 16 * sizeof(float));
        delete[] trans;
    }

    void transform(float* points, uint32_t nPoints) {
        for (uint32_t i = 0; i < nPoints; i++) {
            uint32_t idx = i * 3;
            float x = points[idx];
            float y = points[idx + 1];
            float z = points[idx + 2];
            float newX = matrix[0] * x + matrix[4] * y + matrix[8] * z + matrix[12];
            float newY = matrix[1] * x + matrix[5] * y + matrix[9] * z + matrix[13];
            points[idx + 2] = matrix[2] * x + matrix[6] * y + matrix[10] * z + matrix[14];
            points[idx] = newX;
            points[idx + 1] = newY;
        }
    }

    float* toColumnMajor() const {
        float* out = new float[16];
        out[0] = matrix[0];  out[1] = matrix[4];  out[2] = matrix[8];   out[3] = matrix[12];
        out[4] = matrix[1];  out[5] = matrix[5];  out[6] = matrix[9];   out[7] = matrix[13];
        out[8] = matrix[2];  out[9] = matrix[6];  out[10] = matrix[10]; out[11] = matrix[14];
        out[12] = matrix[3]; out[13] = matrix[7]; out[14] = matrix[11]; out[15] = matrix[15];
        return out;
    }

    FloatBuffer& toFloatBuffer(FloatBuffer& buffer) {
        buffer.put(matrix, 16);
        return buffer;
    }

private:
    bool invertMatrix(const float* m, float* invOut) {
        float* inv = new float[16];
        float det;

        inv[0] = m[5] * m[10] * m[15]
                - m[5] * m[11] * m[14]
                - m[9] * m[6] * m[15]
                + m[9] * m[7] * m[14]
                + m[13] * m[6] * m[11]
                - m[13] * m[7] * m[10];

        inv[4] = -m[4] * m[10] * m[15]
                + m[4] * m[11] * m[14]
                + m[8] * m[6] * m[15]
                - m[8] * m[7] * m[14]
                - m[12] * m[6] * m[11]
                + m[12] * m[7] * m[10];

        inv[8] = m[4] * m[9] * m[15]
                - m[4] * m[11] * m[13]
                - m[8] * m[5] * m[15]
                + m[8] * m[7] * m[13]
                + m[12] * m[5] * m[11]
                - m[12] * m[7] * m[9];

        inv[12] = -m[4] * m[9] * m[14]
                + m[4] * m[10] * m[13]
                + m[8] * m[5] * m[14]
                - m[8] * m[6] * m[13]
                - m[12] * m[5] * m[10]
                + m[12] * m[6] * m[9];

        inv[1] = -m[1] * m[10] * m[15]
                + m[1] * m[11] * m[14]
                + m[9] * m[2] * m[15]
                - m[9] * m[3] * m[14]
                - m[13] * m[2] * m[11]
                + m[13] * m[3] * m[10];

        inv[5] = m[0] * m[10] * m[15]
                - m[0] * m[11] * m[14]
                - m[8] * m[2] * m[15]
                + m[8] * m[3] * m[14]
                + m[12] * m[2] * m[11]
                - m[12] * m[3] * m[10];

        inv[9] = -m[0] * m[9] * m[15]
                + m[0] * m[11] * m[13]
                + m[8] * m[1] * m[15]
                - m[8] * m[3] * m[13]
                - m[12] * m[1] * m[11]
                + m[12] * m[3] * m[9];

        inv[13] = m[0] * m[9] * m[14]
                - m[0] * m[10] * m[13]
                - m[8] * m[1] * m[14]
                + m[8] * m[2] * m[13]
                + m[12] * m[1] * m[10]
                - m[12] * m[2] * m[9];

        inv[2] = m[1] * m[6] * m[15]
                - m[1] * m[7] * m[14]
                - m[5] * m[2] * m[15]
                + m[5] * m[3] * m[14]
                + m[13] * m[2] * m[7]
                - m[13] * m[3] * m[6];

        inv[6] = -m[0] * m[6] * m[15]
                + m[0] * m[7] * m[14]
                + m[4] * m[2] * m[15]
                - m[4] * m[3] * m[14]
                - m[12] * m[2] * m[7]
                + m[12] * m[3] * m[6];

        inv[10] = m[0] * m[5] * m[15]
                - m[0] * m[7] * m[13]
                - m[4] * m[1] * m[15]
                + m[4] * m[3] * m[13]
                + m[12] * m[1] * m[7]
                - m[12] * m[3] * m[5];

        inv[14] = -m[0] * m[5] * m[14]
                + m[0] * m[6] * m[13]
                + m[4] * m[1] * m[14]
                - m[4] * m[2] * m[13]
                - m[12] * m[1] * m[6]
                + m[12] * m[2] * m[5];

        inv[3] = -m[1] * m[6] * m[11]
                + m[1] * m[7] * m[10]
                + m[5] * m[2] * m[11]
                - m[5] * m[3] * m[10]
                - m[9] * m[2] * m[7]
                + m[9] * m[3] * m[6];

        inv[7] = m[0] * m[6] * m[11]
                - m[0] * m[7] * m[10]
                - m[4] * m[2] * m[11]
                + m[4] * m[3] * m[10]
                + m[8] * m[2] * m[7]
                - m[8] * m[3] * m[6];

        inv[11] = -m[0] * m[5] * m[11]
                + m[0] * m[7] * m[9]
                + m[4] * m[1] * m[11]
                - m[4] * m[3] * m[9]
                - m[8] * m[1] * m[7]
                + m[8] * m[3] * m[5];

        inv[15] = m[0] * m[5] * m[10]
                - m[0] * m[6] * m[9]
                - m[4] * m[1] * m[10]
                + m[4] * m[2] * m[9]
                + m[8] * m[1] * m[6]
                - m[8] * m[2] * m[5];

        det = m[0] * inv[0] + m[1] * inv[4] + m[2] * inv[8] + m[3] * inv[12];

        if (det == 0) {
            return false;
        }

        det = 1.0f / det;
        for (int i = 0; i < 16; i++) {
            invOut[i] = inv[i] * det;
        }
        return true;
    }
};

#endif
