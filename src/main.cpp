
#include <chrono>
#include <iostream>

#include "display.h"
#include "painter.h"
#include "geo/vec3f.h"

int main() {
    // GLFW and OpenGL initialization
    display::initialize();
    Painter painter;

    const std::vector<mat4> mCubes = {
            mat4::translate(0.0f, -1.0f, 0.0f) * mat4::scale(2.0f, 0.5f, 2.0f),
            mat4::scale(0.5f, 0.5f, 0.5f),
            mat4::translate(-1.0f, 1.0f, -1.0f) * mat4::scale(0.3f, 0.3f, 0.3f),
            mat4::translate(1.0f, 0.8f, -1.2f) * mat4::scale(0.2f, 0.2f, 0.2f),
            mat4::translate(0.2f, 0.7f, 0.8f) * mat4::scale(0.1f, 0.1f, 0.1f),
            mat4::translate(-1.0f, 0.25f, 0.8f) * mat4::scale(0.1f, 0.1f, 0.1f),
            mat4::translate(-0.9f, 0.35f, -0.5f) * mat4::scale(0.1f, 0.1f, 0.1f),
            mat4::translate(1.2f, 0.3f, -0.2f) * mat4::scale(0.1f, 0.1f, 0.1f),
    };

    const std::vector<vec4f> cCubes = {
            {0.5f, 0.5f, 0.5f, 1.0f},
            {1.0f, 1.0f, 1.0f, 1.0f},
            {0.5f, 1.0f, 0.5f, 1.0f},
            {1.0f, 0.5f, 0.5f, 1.0f},
            {1.0f, 1.0f, 0.5f, 1.0f},
            {1.0f, 0.5f, 1.0f, 1.0f},
            {0.5f, 0.5f, 1.0f, 1.0f},
            {0.5f, 1.0f, 1.0f, 1.0f},
    };

    std::chrono::high_resolution_clock::time_point timeLast = std::chrono::high_resolution_clock::now(); // Used to calculate per-tick deltas

    float timer = 0.0f;

    while(!display::isExiting()) {
        // Calculate 'delta'; the time in seconds since the last update
        std::chrono::high_resolution_clock::time_point timeNow = std::chrono::high_resolution_clock::now();
        float delta = (float)std::chrono::duration_cast<std::chrono::nanoseconds>(timeNow - timeLast).count() / 1000000000.0f;
        timeLast = timeNow;

        display::preUpdate();

        timer += delta;

        painter.draw({cosf(timer) * 4.0f, 5.0f, sinf(timer) * 4.0f},
                     mat4::perspective(60.0f, 1.0f, 0.5f, 20.0f),
                     mat4::lookAt({cosf(timer / 5.0f) * 3.0f, 1.5f, sinf(timer / 5.0f) * 3.0f}),
                     mat4::perspective(90.0f, 16.0f / 9.0f, 1.0f, 10.0f),
                     mCubes, cCubes);

        display::postUpdate();
    }

    // The user has requested the application be closed, so exit
    display::close();
    return 0;
}
