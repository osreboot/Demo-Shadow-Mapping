#pragma once

#include "shader.h"
#include "geo/mat4.h"

class Painter {

private:
    GLuint idVertexBuffer;
    GLuint idFrameBuffer;
    GLuint idTextureDepth;

    Shader shaderLight;
    Shader shaderShadow;

public:
    Painter();
    ~Painter();

    void draw(mat4 vpLight, mat4 vpScene, const std::vector<mat4>& mCubes, const std::vector<vec4f>& cCubes);

};
