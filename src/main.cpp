
#include <chrono>
#include <iostream>

#include "display.h"
#include "painter.h"
#include "geo/vec3f.h"

int main() {
    // GLFW and OpenGL initialization
    display::initialize();
    painter::initialize();

    Shader shaderShadow("res/shadow_mapped.vert", "res/shadow_mapped.frag", [](const GLuint& idProgram){});

    std::chrono::high_resolution_clock::time_point timeLast = std::chrono::high_resolution_clock::now(); // Used to calculate per-tick deltas

    float timer = 0.0f;

    while(!display::isExiting()) {
        // Calculate 'delta'; the time in seconds since the last update
        std::chrono::high_resolution_clock::time_point timeNow = std::chrono::high_resolution_clock::now();
        float delta = (float)std::chrono::duration_cast<std::chrono::nanoseconds>(timeNow - timeLast).count() / 1000000000.0f;
        timeLast = timeNow;

        display::preUpdate();

        timer += delta;

        mat4 model = mat4::translate(0.0f, 0.0f, 0.0f);

        // Source: https://learn.microsoft.com/en-us/previous-versions/windows/desktop/bb281711(v=vs.85)
        vec3f cam(cosf(timer) * 10.0f, 2.0f, sinf(timer) * 10.0f);
        vec3f up(0.0f, 1.0f, 0.0f);
        vec3f laz = vec3f::normalize(cam);
        vec3f lax = vec3f::normalize(vec3f::cross(up, laz));
        vec3f lay = vec3f::cross(laz, lax);
        mat4 view = {
                lax.x, lax.y, lax.z, -vec3f::dot(lax, cam),
                lay.x, lay.y, lay.z, -vec3f::dot(lay, cam),
                laz.x, laz.y, laz.z, -vec3f::dot(laz, cam),
                0.0f, 0.0f, 0.0f, 1.0f
        };

        // Source #1: https://learn.microsoft.com/en-us/previous-versions/windows/desktop/bb281727(v=vs.85)
        // Source #2: https://gamedev.stackexchange.com/questions/120338/what-does-a-perspective-projection-matrix-look-like-in-opengl
        float fov = 90.0f * (float)M_PI / 180.0f;
        float zFar = 100.0f;
        float zNear = 0.1f;
        float aspect = 16.0f / 9.0f;
        mat4 projection = {
                1.0f / (aspect * tanf(fov / 2.0f)), 0.0f, 0.0f, 0.0f,
                0.0f, 1.0f / tanf(fov / 2.0f), 0.0f, 0.0f,
                0.0f, 0.0f, -(zFar + zNear) / (zFar - zNear), -(2.0f * zFar * zNear) / (zFar - zNear),
                0.0f, 0.0f, -1.0f, 0.0f
        };

        painter::draw(projection * view * model, shaderShadow, {1.0f, 0.0f, 0.0f, 1.0f});

        display::postUpdate();
    }

    // The user has requested the application be closed, so exit
    display::close();
    return 0;
}
