
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

    // Enable alpha blending
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // Reserve buffer for cube vertices
    idVertexBuffer = 0;
    glGenBuffers(1, &idVertexBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, idVertexBuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(VERTICES_CUBE), VERTICES_CUBE, GL_STATIC_DRAW);

    // Calculate normals of cube triangles
    std::vector<GLfloat> normalsCube(sizeof(VERTICES_CUBE) / sizeof(GLfloat), 0.0f);
    for(int i = 0; i < sizeof(VERTICES_CUBE) / sizeof(GLfloat); i += 9){
        vec3f p0 = {VERTICES_CUBE[i], VERTICES_CUBE[i + 1], VERTICES_CUBE[i + 2]};
        vec3f p1 = {VERTICES_CUBE[i + 3], VERTICES_CUBE[i + 4], VERTICES_CUBE[i + 5]};
        vec3f p2 = {VERTICES_CUBE[i + 6], VERTICES_CUBE[i + 7], VERTICES_CUBE[i + 8]};
        vec3f n = vec3f::normalize(vec3f::cross(p0 - p1, p2 - p1));
        normalsCube[i] = n.x;
        normalsCube[i + 1] = n.y;
        normalsCube[i + 2] = n.z;
        normalsCube[i + 3] = n.x;
        normalsCube[i + 4] = n.y;
        normalsCube[i + 5] = n.z;
        normalsCube[i + 6] = n.x;
        normalsCube[i + 7] = n.y;
        normalsCube[i + 8] = n.z;
    }

    // Reserve buffer for cube normals
    idNormalBuffer = 0;
    glGenBuffers(1, &idNormalBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, idNormalBuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(VERTICES_CUBE), &normalsCube[0], GL_STATIC_DRAW);

    // Enable depth testing
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);

    // Enable backface culling
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);

    // Reserve framebuffer for lightmap depth testing
    idFrameBuffer = 0;
    glGenFramebuffers(1, &idFrameBuffer);
    glBindFramebuffer(GL_FRAMEBUFFER, idFrameBuffer);

    // Create lightmap depth texture
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

    // Attach lightmap depth texture to lightmap frame buffer
    glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, idTextureDepth, 0);

    glDrawBuffer(GL_NONE);
}

Painter::~Painter() = default;

void Painter::draw(vec3f lLight, mat4 pLight, mat4 vScene, mat4 pScene,
                   const std::vector<mat4>& mCubes, const std::vector<vec4f>& cCubes) {
    mat4 vLight = mat4::lookAt(lLight);

    // Bind lightmap framebuffer
    glBindFramebuffer(GL_FRAMEBUFFER, idFrameBuffer);
    glViewport(0, 0, 4096, 4096);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glUseProgram(shaderLight.getProgramId());

    // Bind the cube vertices
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, idVertexBuffer);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, nullptr);

    // Render cubes
    for(mat4 mCube : mCubes) {
        mat4 mvpLight = pLight * vLight * mCube;
        glUniformMatrix4fv(glGetUniformLocation(shaderLight.getProgramId(), "mvp"), 1, GL_TRUE, &mvpLight[0][0]);

        // Draw the cube
        glDrawArrays(GL_TRIANGLES, 0, sizeof(VERTICES_CUBE) / (3 * sizeof(GLfloat)));
    }

    // Unbind cube vertices
    glDisableVertexAttribArray(0);

    // Bind default framebuffer
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glViewport(0, 0, display::getSize().x, display::getSize().y);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glUseProgram(shaderShadow.getProgramId());

    // Use the lightmap depth texture
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, idTextureDepth);
    glUniform1i(glGetUniformLocation(shaderShadow.getProgramId(), "textureShadow"), 0);

    // Bind the cube vertices
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, idVertexBuffer);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, nullptr);

    // Bind the cube normals
    glEnableVertexAttribArray(1);
    glBindBuffer(GL_ARRAY_BUFFER, idNormalBuffer);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, nullptr);

    // Render cubes
    for(int i = 0; i < mCubes.size(); i++) {
        mat4 m = mCubes[i];
        mat4 mvpScene = pScene * vScene * m;
        mat4 mvpLightBias = mat4::translate(0.5f, 0.5f, 0.5f) * mat4::scale(0.5f, 0.5f, 0.5f) * pLight * vLight * m;

        glUniformMatrix4fv(glGetUniformLocation(shaderShadow.getProgramId(), "mvpScene"), 1, GL_TRUE, &mvpScene[0][0]);
        glUniformMatrix4fv(glGetUniformLocation(shaderShadow.getProgramId(), "m"), 1, GL_TRUE, &m[0][0]);
        glUniformMatrix4fv(glGetUniformLocation(shaderShadow.getProgramId(), "vScene"), 1, GL_TRUE, &vScene[0][0]);
        glUniformMatrix4fv(glGetUniformLocation(shaderShadow.getProgramId(), "mvpLightBias"), 1, GL_TRUE, &mvpLightBias[0][0]);
        glUniform3f(glGetUniformLocation(shaderShadow.getProgramId(), "lLight"), lLight.x, lLight.y, lLight.z);
        glUniform4f(glGetUniformLocation(shaderShadow.getProgramId(), "colorize"), cCubes[i][0], cCubes[i][1], cCubes[i][2], cCubes[i][3]);

        // Draw the cube
        glDrawArrays(GL_TRIANGLES, 0, sizeof(VERTICES_CUBE) / (3 * sizeof(GLfloat)));
    }

    // Unbind cube vertices
    glDisableVertexAttribArray(0);
    glDisableVertexAttribArray(1);
}
