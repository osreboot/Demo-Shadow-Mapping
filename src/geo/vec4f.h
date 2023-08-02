#pragma once

#include <stdexcept>

class vec4f {

private:
    float arr[4] = {};

public:

    vec4f() = default;
    vec4f(float x, float y, float z, float w) : arr{x, y, z, w} {}

    float operator[](int index) const {
        if(index < 0 || index >= 4) throw std::runtime_error("Vector index out of bounds!");
        return arr[index];
    }

    float& operator[](int index) {
        if(index < 0 || index >= 4) throw std::runtime_error("Vector index out of bounds!");
        return arr[index];
    }

};
