#pragma once

#include <stdexcept>
#include <cmath>

class vec3f {

private:

public:
    float x = 0.0f, y = 0.0f, z = 0.0f;

    vec3f() = default;
    vec3f(float x, float y, float z) : x(x), y(y), z(z) {}

    static vec3f normalize(vec3f in) {
        float mag = sqrtf(in.x * in.x + in.y * in.y + in.z * in.z);
        return {in.x / mag, in.y / mag, in.z / mag};
    }

    static float dot(vec3f a, vec3f b) {
        return a.x * b.x + a.y * b.y + a.z * b.z;
    }

    static vec3f cross(vec3f a, vec3f b) {
        return {
            a.y * b.z - a.z * b.y,
            a.z * b.x - a.x * b.z,
            a.x * b.y - a.y * b.x
        };
    }

    vec3f operator+(const vec3f& other) const { return {x + other.x, y + other.y, z + other.z}; }
    vec3f operator-() const { return {-x, -y, -z}; }
    vec3f operator-(const vec3f& other) const { return {x - other.x, y - other.y, z - other.z}; }
    vec3f operator*(const vec3f& other) const { return {x * other.x, y * other.y, z * other.z}; }
    vec3f operator/(const vec3f& other) const { return {x / other.x, y / other.y, z / other.z}; }

};
