#version 330 core

layout(location = 0) in vec3 vertex;

out vec4 shadowCoord;

uniform mat4 mvp;
uniform mat4 mvpBias;

void main() {
    gl_Position = mvp * vec4(vertex, 1.0);

    shadowCoord = mvpBias * vec4(vertex, 1.0);
}
