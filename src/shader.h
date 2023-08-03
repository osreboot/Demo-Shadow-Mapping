#pragma once

#include <functional>

#include <GL/glew.h>

// Represents an OpenGL shader program.
class Shader {

private:
    GLuint idProgram;

public:
    // Disable copying. This is necessary because we need to keep this instance consistent with the shader program ID
    // reserved within the OpenGL context.
    Shader(Shader const&) = delete;
    Shader& operator=(Shader const&) = delete;

    // 'pathVertex' and 'pathFragment' are the complete file paths to the shader program files, including file names
    // and extensions.
    Shader(const char* pathVertex, const char* pathFragment);
    ~Shader();

    const GLuint& getProgramId() const { return idProgram; }

};
