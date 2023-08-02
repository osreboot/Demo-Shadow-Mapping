#pragma once

class vec2f {

private:

public:
    float x = 0.0f, y = 0.0f;

    vec2f() = default;
    vec2f(float x, float y) : x(x), y(y) {}

    vec2f operator+(const vec2f& other) const { return {x + other.x, y + other.y}; }
    vec2f operator-(const vec2f& other) const { return {x - other.x, y - other.y}; }
    vec2f operator*(const vec2f& other) const { return {x * other.x, y * other.y}; }
    vec2f operator/(const vec2f& other) const { return {x / other.x, y / other.y}; }

};
