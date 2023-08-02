#version 330 core

out vec4 color;

uniform vec4 colorize;

void main() {
    color = colorize;
}
