#pragma once

#include <functional>

#include <GL/glew.h>

// Represents an OpenGL shader program.
class Shader {

private:
    GLuint idProgram;

    // Function for sending extra parameters to the shader program. Automatically invoked once each time the program is
    // executed.
    std::function<void(const GLuint&)> funcCustomParams;

public:
    // Disable copying. This is necessary because we need to keep this instance consistent with the shader program ID
    // reserved within the OpenGL context.
    Shader(Shader const&) = delete;
    Shader& operator=(Shader const&) = delete;

    // 'pathVertex' and 'pathFragment' are the complete file paths to the shader program files, including file names
    // and extensions.
    Shader(const char* pathVertex, const char* pathFragment, const std::function<void(const GLuint&)>& funcCustomParams);
    ~Shader();

    // Invokes the function for sending additional parameters to the shader program.
    void applyCustomParameters() const { funcCustomParams(idProgram); }

    const GLuint& getProgramId() const { return idProgram; }

};
