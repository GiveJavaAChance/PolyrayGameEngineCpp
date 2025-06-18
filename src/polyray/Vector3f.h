#ifndef VECTOR3F_H_INCLUDED
#define VECTOR3F_H_INCLUDED

#pragma once
#include <cmath>
#include <iostream>

struct Vector3f {
    float x, y, z;

    Vector3f() : x(0.0f), y(0.0f), z(0.0f) {}
    Vector3f(float x, float y, float z) : x(x), y(y), z(z) {}
    Vector3f(const Vector3f& v) : x(v.x), y(v.y), z(v.z) {}

    Vector3f operator+(const Vector3f& other) const {
        return Vector3f(x + other.x, y + other.y, z + other.z);
    }

    Vector3f operator-(const Vector3f& other) const {
        return Vector3f(x - other.x, y - other.y, z - other.z);
    }

    Vector3f operator*(float scalar) const {
        return Vector3f(x * scalar, y * scalar, z * scalar);
    }

    Vector3f operator/(float scalar) const {
        return Vector3f(x / scalar, y / scalar, z / scalar);
    }

    Vector3f& operator+=(const Vector3f& other) {
        x += other.x; y += other.y; z += other.z;
        return *this;
    }

    Vector3f& operator-=(const Vector3f& other) {
        x -= other.x; y -= other.y; z -= other.z;
        return *this;
    }

    Vector3f& operator*=(float scalar) {
        x *= scalar; y *= scalar; z *= scalar;
        return *this;
    }

    Vector3f& operator/=(float scalar) {
        x /= scalar; y /= scalar; z /= scalar;
        return *this;
    }

    bool operator==(const Vector3f& other) const {
        return x == other.x && y == other.y && z == other.z;
    }

    float length() const {
        return std::sqrt(x * x + y * y + z * z);
    }

    static Vector3f normalize(const Vector3f& v) {
        float len = v.length();
        if (len == 0.0f) return Vector3f();
        return v / len;
    }

    static Vector3f invert(const Vector3f& v) {
        return Vector3f(-v.x, -v.y, -v.z);
    }

    static float dot(const Vector3f& a, const Vector3f& b) {
        return a.x * b.x + a.y * b.y + a.z * b.z;
    }

    static Vector3f cross(const Vector3f& a, const Vector3f& b) {
        return Vector3f(
            a.y * b.z - a.z * b.y,
            a.z * b.x - a.x * b.z,
            a.x * b.y - a.y * b.x
        );
    }

    static Vector3f lerp(const Vector3f& a, const Vector3f& b, float f) {
        return a + (b - a) * f;
    }

    friend std::ostream& operator<<(std::ostream& os, const Vector3f& v) {
        os << "Vector3f: (" << v.x << ", " << v.y << ", " << v.z << ")";
        return os;
    }
};

inline Vector3f operator*(float scalar, const Vector3f& v) {
    return v * scalar;
}

#endif
