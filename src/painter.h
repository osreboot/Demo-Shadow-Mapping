#pragma once

#include "shader.h"
#include "geo/mat4.h"

class Painter {

private:
    GLuint idVertexBuffer;
    GLuint idNormalBuffer;

    GLuint idFrameBuffer;
    GLuint idTextureDepth;

    Shader shaderLight;
    Shader shaderShadow;

public:
    Painter();
    ~Painter();

    void draw(vec3f lLight, mat4 pLight, mat4 vScene, mat4 pScene, const std::vector<mat4>& mCubes, const std::vector<vec4f>& cCubes);

};
