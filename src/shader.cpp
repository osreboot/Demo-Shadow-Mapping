
#include <iostream>
#include <fstream>
#include <sstream>

#include "shader.h"

Shader::Shader(const char *pathVertex, const char *pathFragment, const std::function<void(const GLuint&)>& funcCustomParams) :
        funcCustomParams(funcCustomParams) {
    GLuint idVertexShader = glCreateShader(GL_VERTEX_SHADER);
    GLuint idFragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

    // Read the vertex shader from file
    std::string codeVertexShader;
    std::ifstream streamVertexShader(pathVertex, std::ios::in);
    if(streamVertexShader.is_open()) {
        std::stringstream sstr;
        sstr << streamVertexShader.rdbuf();
        codeVertexShader = sstr.str();
        streamVertexShader.close();
    }else throw std::runtime_error("error initializing vertex shader");

    // Read the fragment shader from file
    std::string codeFragmentShader;
    std::ifstream streamFragmentShader(pathFragment, std::ios::in);
    if(streamFragmentShader.is_open()) {
        std::stringstream sstr;
        sstr << streamFragmentShader.rdbuf();
        codeFragmentShader = sstr.str();
        streamFragmentShader.close();
    }else throw std::runtime_error("error initializing fragment shader");

    GLint compileResult = GL_FALSE;
    int compileLogLength;

    // Compile the vertex shader
    char const* pointerVertexCode = codeVertexShader.c_str();
    glShaderSource(idVertexShader, 1, &pointerVertexCode, nullptr);
    glCompileShader(idVertexShader);

    // Print vertex shader errors
    glGetShaderiv(idVertexShader, GL_COMPILE_STATUS, &compileResult);
    glGetShaderiv(idVertexShader, GL_INFO_LOG_LENGTH, &compileLogLength);
    if(compileLogLength > 0) {
        std::vector<char> errorMessage(compileLogLength + 1);
        glGetShaderInfoLog(idVertexShader, compileLogLength, nullptr, &errorMessage[0]);
        printf("%s\n", &errorMessage[0]);
    }

    // Compile the fragment shader
    char const* pointerFragmentCode = codeFragmentShader.c_str();
    glShaderSource(idFragmentShader, 1, &pointerFragmentCode, nullptr);
    glCompileShader(idFragmentShader);

    // Print fragment shader errors
    glGetShaderiv(idFragmentShader, GL_COMPILE_STATUS, &compileResult);
    glGetShaderiv(idFragmentShader, GL_INFO_LOG_LENGTH, &compileLogLength);
    if(compileLogLength > 0) {
        std::vector<char> errorMessage(compileLogLength + 1);
        glGetShaderInfoLog(idFragmentShader, compileLogLength, nullptr, &errorMessage[0]);
        printf("%s\n", &errorMessage[0]);
    }

    // Link compiled shaders to the combined program
    idProgram = glCreateProgram();
    glAttachShader(idProgram, idVertexShader);
    glAttachShader(idProgram, idFragmentShader);
    glLinkProgram(idProgram);

    glDetachShader(idProgram, idVertexShader);
    glDetachShader(idProgram, idFragmentShader);

    glDeleteShader(idVertexShader);
    glDeleteShader(idFragmentShader);
}

Shader::~Shader() {
    glDeleteProgram(idProgram);
}
