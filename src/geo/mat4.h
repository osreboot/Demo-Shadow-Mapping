#pragma once

#include <algorithm>
#include <cmath>

#include "vec4f.h"
#include "vec3f.h"

class mat4 {

private:
    vec4f arr[4] = {};

public:

    mat4() = default;
    mat4(vec4f v0, vec4f v1, vec4f v2, vec4f v3) : arr{v0, v1, v2, v3} {};
    mat4(float x0, float y0, float z0, float w0,
         float x1, float y1, float z1, float w1,
         float x2, float y2, float z2, float w2,
         float x3, float y3, float z3, float w3) : arr{
        {x0, y0, z0, w0},
        {x1, y1, z1, w1},
        {x2, y2, z2, w2},
        {x3, y3, z3, w3}} {};

    vec4f& operator[](int index) {
        if(index < 0 || index >= 4) throw std::runtime_error("Matrix index out of bounds!");
        return arr[index];
    }

    mat4 operator*(mat4 o) {
        mat4 out;
        for(int y = 0; y < 4; y++){
            for(int x = 0; x < 4; x++){
                for(int i = 0; i < 4; i++){
                    out[y][x] += arr[y][i] * o[i][x];
                }
            }
        }
        return out;
    }

    vec3f operator*(vec3f v){
        vec4f appended = {v.x, v.y, v.z, 1.0f};
        vec4f out;
        for(int y = 0; y < 4; y++){
            for(int x = 0; x < 4; x++){
                for(int i = 0; i < 4; i++){
                    out[y] += arr[y][i] * appended[i];
                }
            }
        }
        return {out[0] / appended[3], out[1] / appended[3], out[2] / appended[3]};
    }

    static mat4 translate(float x, float y, float z) {
        return {1.0f, 0.0f, 0.0f, x,
                0.0f, 1.0f, 0.0f, y,
                0.0f, 0.0f, 1.0f, z,
                0.0f, 0.0f, 0.0f, 1.0f};
    }

    static mat4 scale(float x, float y, float z) {
        return {x, 0.0f, 0.0f, 0.0f,
                0.0f, y, 0.0f, 0.0f,
                0.0f, 0.0f, z, 0.0f,
                0.0f, 0.0f, 0.0f, 1.0f};
    }

    static mat4 rotate(float y, float p, float r) {
        mat4 yaw(cosf(y), -sinf(y), 0.0f, 0.0f,
                 sinf(y), cosf(y), 0.0f, 0.0f,
                 0.0f, 0.0f, 1.0f, 0.0f,
                 0.0f, 0.0f, 0.0f, 1.0f);
        mat4 pitch(cosf(p), 0.0f, sinf(p), 0.0f,
                   0.0f, 1.0f, 0.0f, 0.0f,
                   -sinf(p), 0.0f, cosf(p), 0.0f,
                   0.0f, 0.0f, 0.0f, 1.0f);
        mat4 roll(1.0f, 0.0f, 0.0f, 0.0f,
                  0.0f, cosf(r), -sinf(r), 0.0f,
                  0.0f, sinf(r), cosf(r), 0.0f,
                  0.0f, 0.0f, 0.0f, 1.0f);
        return yaw * pitch * roll;
    }

    static mat4 transpose(mat4 mat) {
        return {
                mat[0][0], mat[1][0], mat[2][0], mat[3][0],
                mat[0][1], mat[1][1], mat[2][1], mat[3][1],
                mat[0][2], mat[1][2], mat[2][2], mat[3][2],
                mat[0][3], mat[1][3], mat[2][3], mat[3][3]
        };
    }

};
