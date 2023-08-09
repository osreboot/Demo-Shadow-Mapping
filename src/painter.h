#pragma once

#include "shader.h"
#include "geo/mat4.h"

class Painter {

private:
    GLuint idVertexBuffer; // Cube vertices
    GLuint idNormalBuffer; // Cube vertex normals

    GLuint idFrameBuffer; // Lightmap depth framebuffer
    GLuint idTextureDepth; // Lightmap depth texture

    Shader shaderLight; // Passthrough shader for lightmap depth testing
    Shader shaderShadow; // Shader that renders an illuminated scene with lightmap-dictated shadows

public:
    Painter();
    ~Painter();

    // Render the scene. Requires light position, light and scene transformation matrices, cube transformation matrices,
    // and cube colors.
    void draw(vec3f lLight, mat4 pLight, mat4 vScene, mat4 pScene, const std::vector<mat4>& mCubes, const std::vector<vec4f>& cCubes);

};
