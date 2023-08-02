#pragma once

class vec2i {

private:

public:
    int x = 0, y = 0;

    vec2i() = default;
    vec2i(int x, int y) : x(x), y(y) {}

    vec2i operator+(const vec2i& other) const { return {x + other.x, y + other.y}; }
    vec2i operator-(const vec2i& other) const { return {x - other.x, y - other.y}; }
    vec2i operator*(const vec2i& other) const { return {x * other.x, y * other.y}; }
    vec2i operator/(const vec2i& other) const { return {x / other.x, y / other.y}; }

};
