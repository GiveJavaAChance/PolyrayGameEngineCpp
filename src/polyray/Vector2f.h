#ifndef VECTOR2F_H_INCLUDED
#define VECTOR2F_H_INCLUDED

#pragma once
#include <cmath>
#include <iostream>

struct Vector2f {
    float x, y;

    Vector2f() : x(0.0f), y(0.0f) {}
    Vector2f(float x, float y) : x(x), y(y) {}
    Vector2f(const Vector2f& v) : x(v.x), y(v.y) {}

    Vector2f operator+(const Vector2f& other) const {
        return Vector2f(x + other.x, y + other.y);
    }

    Vector2f operator-(const Vector2f& other) const {
        return Vector2f(x - other.x, y - other.y);
    }

    Vector2f operator*(float scalar) const {
        return Vector2f(x * scalar, y * scalar);
    }

    Vector2f operator/(float scalar) const {
        return Vector2f(x / scalar, y / scalar);
    }

    Vector2f& operator+=(const Vector2f& other) {
        x += other.x;
        y += other.y;
        return *this;
    }

    Vector2f& operator-=(const Vector2f& other) {
        x -= other.x;
        y -= other.y;
        return *this;
    }

    Vector2f& operator*=(float scalar) {
        x *= scalar;
        y *= scalar;
        return *this;
    }

    Vector2f& operator/=(float scalar) {
        x /= scalar;
        y /= scalar;
        return *this;
    }

    bool operator==(const Vector2f& other) const {
        return x == other.x && y == other.y;
    }

    float length() const {
        return std::sqrt(x * x + y * y);
    }

    static Vector2f normalize(const Vector2f& v) {
        float len = v.length();
        if (len == 0.0f) return Vector2f();
        return v / len;
    }

    static Vector2f invert(const Vector2f& v) {
        return Vector2f(-v.x, -v.y);
    }

    static float dot(const Vector2f& a, const Vector2f& b) {
        return a.x * b.x + a.y * b.y;
    }

    static Vector2f lerp(const Vector2f& a, const Vector2f& b, float f) {
        return a + (b - a) * f;
    }

    friend std::ostream& operator<<(std::ostream& os, const Vector2f& v) {
        os << "Vector2f: (" << v.x << ", " << v.y << ")";
        return os;
    }
};

inline Vector2f operator*(float scalar, const Vector2f& v) {
    return v * scalar;
}

#endif
