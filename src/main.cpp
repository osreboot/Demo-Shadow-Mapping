
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
        mat4 view = mat4::lookAt({cosf(timer) * 10.0f, 2.0f, sinf(timer) * 10.0f});
        mat4 projection = mat4::perspective(90.0f, 16.0f / 9.0f, 0.1f, 100.0f);

        painter::draw(projection * view * model, shaderShadow, {1.0f, 0.0f, 0.0f, 1.0f});

        display::postUpdate();
    }

    // The user has requested the application be closed, so exit
    display::close();
    return 0;
}
