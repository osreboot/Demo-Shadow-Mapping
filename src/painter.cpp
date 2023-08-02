
#include <GL/glew.h>

#include "painter.h"
#include "geo/mat4.h"

namespace painter {

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

    GLuint idVertexBuffer = -1;

    void initialize() {
        // Reserve a vertex array for quad rendering
        GLuint idVertexArray;
        glGenVertexArrays(1, &idVertexArray);
        glBindVertexArray(idVertexArray);

        // Enable alpha blending
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        glGenBuffers(1, &idVertexBuffer);
        glBindBuffer(GL_ARRAY_BUFFER, idVertexBuffer);
        glBufferData(GL_ARRAY_BUFFER, sizeof(VERTICES_CUBE), VERTICES_CUBE, GL_STATIC_DRAW);
    }

    void draw(mat4 matrix, const Shader& shader, const Color& color) {
        glUseProgram(shader.getProgramId());

        GLint idMatrixMvp = glGetUniformLocation(shader.getProgramId(), "mvp");
        glUniformMatrix4fv(idMatrixMvp, 1, GL_TRUE, &matrix[0][0]);

        // Send 'color' to the shader program
        GLint idColor = glGetUniformLocation(shader.getProgramId(), "colorize");
        glUniform4f(idColor, color.r, color.g, color.b, color.a);

        // Send other shader-specific parameters to the shader program
        shader.applyCustomParameters();

        // Bind the cube vertices
        glEnableVertexAttribArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, idVertexBuffer);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, nullptr);

        // Draw the cube
        glDrawArrays(GL_TRIANGLES, 0, sizeof(VERTICES_CUBE) / (3 * sizeof(GLfloat)));

        // Unbind cube vertices
        glDisableVertexAttribArray(0);
    }

}
