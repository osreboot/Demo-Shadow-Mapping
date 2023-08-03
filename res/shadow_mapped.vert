#version 330 core

layout(location = 0) in vec3 mVertex;
layout(location = 1) in vec3 mVertexNormal;

out vec3 lVertex;
out vec3 dSceneVertex;
out vec3 dSceneLight;
out vec3 nSceneVertex;
out vec4 shadowCoord;

uniform mat4 mvpScene;
uniform mat4 m;
uniform mat4 vScene;
uniform mat4 mvpLightBias;
uniform vec3 lLight;

void main() {
    gl_Position = mvpScene * vec4(mVertex, 1.0);

    // Light-space position of model vertex
    shadowCoord = mvpLightBias * vec4(mVertex, 1.0);

    // World-space position of model vertex
    lVertex = (m * vec4(mVertex, 1.0)).xyz;

    // Direction from camera to vertex
    dSceneVertex = vec3(0.0) - (vScene * m * vec4(mVertex, 1.0)).xyz;

    // Direction from camera to light
    dSceneLight = vec3(0.0) - (vScene * vec4(lLight, 0.0)).xyz;

    // World-space normal of vertex
    nSceneVertex = (vScene * m * vec4(mVertexNormal, 0.0)).xyz;
}
