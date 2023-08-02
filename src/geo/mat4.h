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

    // Source: https://en.wikipedia.org/wiki/Rotation_matrix
    static mat4 rotate(float x, float y, float z) {
        mat4 yaw(cosf(x), -sinf(x), 0.0f, 0.0f,
                 sinf(x), cosf(x), 0.0f, 0.0f,
                 0.0f, 0.0f, 1.0f, 0.0f,
                 0.0f, 0.0f, 0.0f, 1.0f);
        mat4 pitch(cosf(y), 0.0f, sinf(y), 0.0f,
                   0.0f, 1.0f, 0.0f, 0.0f,
                   -sinf(y), 0.0f, cosf(y), 0.0f,
                   0.0f, 0.0f, 0.0f, 1.0f);
        mat4 roll(1.0f, 0.0f, 0.0f, 0.0f,
                  0.0f, cosf(z), -sinf(z), 0.0f,
                  0.0f, sinf(z), cosf(z), 0.0f,
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

    // Source: https://learn.microsoft.com/en-us/previous-versions/windows/desktop/bb281711(v=vs.85)
    static mat4 lookAt(vec3f camera) {
        static const vec3f up(0.0f, 1.0f, 0.0f);
        vec3f laz = vec3f::normalize(camera);
        vec3f lax = vec3f::normalize(vec3f::cross(up, laz));
        vec3f lay = vec3f::cross(laz, lax);
        return {
                lax.x, lax.y, lax.z, -vec3f::dot(lax, camera),
                lay.x, lay.y, lay.z, -vec3f::dot(lay, camera),
                laz.x, laz.y, laz.z, -vec3f::dot(laz, camera),
                0.0f, 0.0f, 0.0f, 1.0f
        };
    }

    // Source #1: https://learn.microsoft.com/en-us/previous-versions/windows/desktop/bb281727(v=vs.85)
    // Source #2: https://gamedev.stackexchange.com/questions/120338/what-does-a-perspective-projection-matrix-look-like-in-opengl
    static mat4 perspective(float fovDeg, float aspect, float zNear, float zFar) {
        return {
                1.0f / (aspect * tanf((fovDeg * (float)M_PI / 180.0f) / 2.0f)), 0.0f, 0.0f, 0.0f,
                0.0f, 1.0f / tanf((fovDeg * (float)M_PI / 180.0f) / 2.0f), 0.0f, 0.0f,
                0.0f, 0.0f, -(zFar + zNear) / (zFar - zNear), -(2.0f * zFar * zNear) / (zFar - zNear),
                0.0f, 0.0f, -1.0f, 0.0f
        };
    }

};
