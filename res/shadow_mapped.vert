#version 330 core

layout(location = 0) in vec3 vertex;

out vec4 shadowCoord;

uniform mat4 mvpScene;
uniform mat4 mvpLightBias;

void main() {
    gl_Position = mvpScene * vec4(vertex, 1.0);

    shadowCoord = mvpLightBias * vec4(vertex, 1.0);
}
