#pragma once

#include "shader.h"
#include "geo/mat4.h"

// Represents a RGBA color with value ranges of 0.0 to 1.0
struct Color {
    GLfloat r, g, b, a;
};

namespace painter {

    // Configures the OpenGL context for rendering
    void initialize();

    // Renders a textured, colorized quad to the screen
    void draw(mat4 matrix, const Shader& shader, const Color& color);

}
