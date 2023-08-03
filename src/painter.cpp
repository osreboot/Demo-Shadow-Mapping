
#include <GL/glew.h>

#include "painter.h"
#include "geo/mat4.h"
#include "display.h"


const GLfloat VERTICES_CUBE[] = {
        -1.0f, -1.0f, -1.0f,
        -1.0f, -1.0f,  1.0f,
        -1.0f,  1.0f,  1.0f,
         1.0f,  1.0f, -1.0f,
        -1.0f, -1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,
         1.0f, -1.0f,  1.0f,
        -1.0f, -1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,
         1.0f,  1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,
        -1.0f, -1.0f, -1.0f,
        -1.0f, -1.0f, -1.0f,
        -1.0f,  1.0f,  1.0f,
        -1.0f,  1.0f, -1.0f,
         1.0f, -1.0f,  1.0f,
        -1.0f, -1.0f,  1.0f,
        -1.0f, -1.0f, -1.0f,
        -1.0f,  1.0f,  1.0f,
        -1.0f, -1.0f,  1.0f,
         1.0f, -1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f, -1.0f, -1.0f,
         1.0f,  1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f, -1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,
         1.0f,  1.0f,  1.0f,
        -1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
        -1.0f,  1.0f,  1.0f,
         1.0f, -1.0f,  1.0f,
};

Painter::Painter() :
        shaderLight("res/light.vert", "res/light.frag"),
        shaderShadow("res/shadow_mapped.vert", "res/shadow_mapped.frag") {
    GLuint idVertexArray;
    glGenVertexArrays(1, &idVertexArray);
    glBindVertexArray(idVertexArray);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    idVertexBuffer = 0;
    glGenBuffers(1, &idVertexBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, idVertexBuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(VERTICES_CUBE), VERTICES_CUBE, GL_STATIC_DRAW);

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);

    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);

    idFrameBuffer = 0;
    glGenFramebuffers(1, &idFrameBuffer);
    glBindFramebuffer(GL_FRAMEBUFFER, idFrameBuffer);

    idTextureDepth = 0;
    glGenTextures(1, &idTextureDepth);
    glBindTexture(GL_TEXTURE_2D, idTextureDepth);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT16, 4096, 4096, 0, GL_DEPTH_COMPONENT, GL_FLOAT, nullptr);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_FUNC, GL_LEQUAL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE, GL_COMPARE_R_TO_TEXTURE);

    glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, idTextureDepth, 0);

    glDrawBuffer(GL_NONE);
}

Painter::~Painter() = default;

void Painter::draw(mat4 vpLight, mat4 vpScene, const std::vector<mat4>& mCubes, const std::vector<vec4f>& cCubes) {
    glBindFramebuffer(GL_FRAMEBUFFER, idFrameBuffer);
    glViewport(0, 0, 4096, 4096);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glUseProgram(shaderLight.getProgramId());

    // Bind the cube vertices
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, idVertexBuffer);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, nullptr);

    for(mat4 mCube : mCubes) {
        mat4 mvpLight = vpLight * mCube;
        glUniformMatrix4fv(glGetUniformLocation(shaderLight.getProgramId(), "mvp"), 1, GL_TRUE, &mvpLight[0][0]);

        // Draw the cube
        glDrawArrays(GL_TRIANGLES, 0, sizeof(VERTICES_CUBE) / (3 * sizeof(GLfloat)));
    }

    // Unbind cube vertices
    glDisableVertexAttribArray(0);

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glViewport(0, 0, display::getSize().x, display::getSize().y);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glUseProgram(shaderShadow.getProgramId());

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, idTextureDepth);
    glUniform1i(glGetUniformLocation(shaderShadow.getProgramId(), "textureShadow"), 0);

    // Bind the cube vertices
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, idVertexBuffer);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, nullptr);

    for(int i = 0; i < mCubes.size(); i++) {
        mat4 mvpScene = vpScene * mCubes[i];
        mat4 vpLightBias = mat4::translate(0.5f, 0.5f, 0.5f) * mat4::scale(0.5f, 0.5f, 0.5f) * vpLight * mCubes[i];

        glUniformMatrix4fv(glGetUniformLocation(shaderShadow.getProgramId(), "mvpScene"), 1, GL_TRUE, &mvpScene[0][0]);
        glUniformMatrix4fv(glGetUniformLocation(shaderShadow.getProgramId(), "mvpLightBias"), 1, GL_TRUE, &vpLightBias[0][0]);
        glUniform4f(glGetUniformLocation(shaderShadow.getProgramId(), "colorize"), cCubes[i][0], cCubes[i][1], cCubes[i][2], cCubes[i][3]);

        // Draw the cube
        glDrawArrays(GL_TRIANGLES, 0, sizeof(VERTICES_CUBE) / (3 * sizeof(GLfloat)));
    }

    // Unbind cube vertices
    glDisableVertexAttribArray(0);
}
