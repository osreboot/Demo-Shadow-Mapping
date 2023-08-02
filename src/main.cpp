
#include <chrono>
#include <iostream>

#include "display.h"
#include "painter.h"
#include "geo/vec3f.h"

int main() {
    // GLFW and OpenGL initialization
    display::initialize();
    painter::initialize();

    Shader shaderShadow("res/shadow_mapped.vert", "res/shadow_mapped.frag", [](const GLuint& idProgram){});

    std::chrono::high_resolution_clock::time_point timeLast = std::chrono::high_resolution_clock::now(); // Used to calculate per-tick deltas

    float timer = 0.0f;

    while(!display::isExiting()) {
        // Calculate 'delta'; the time in seconds since the last update
        std::chrono::high_resolution_clock::time_point timeNow = std::chrono::high_resolution_clock::now();
        float delta = (float)std::chrono::duration_cast<std::chrono::nanoseconds>(timeNow - timeLast).count() / 1000000000.0f;
        timeLast = timeNow;

        display::preUpdate();

        timer += delta;

        mat4 model = mat4::translate(0.0f, sinf(timer), 0.0f);

        //vec3f cam(cosf(timer) * 0.4f, 0.0f, sinf(timer) * 0.4f);
        vec3f cam(cosf(timer) * 10.0f, 2.0f, sinf(timer) * 10.0f);
        vec3f up(0.0f, 1.0f, 0.0f);
        vec3f laz = vec3f::normalize(-cam);
        vec3f lax = vec3f::normalize(vec3f::cross(up, laz));
        vec3f lay = vec3f::cross(laz, lax);
        mat4 view = {
                lax.x, lax.y, lax.z, -vec3f::dot(lax, cam),
                lay.x, lay.y, lay.z, -vec3f::dot(lay, cam),
                laz.x, laz.y, laz.z, -vec3f::dot(laz, cam),
                0.0f, 0.0f, 0.0f, 1.0f
        };
        //view = mat4::transpose(view);

        float fov = 110.0f * (float)M_PI / 180.0f;
        float zFar = 100.0f;
        float zNear = 0.1f;
        float aspect = 16.0f / 9.0f;
        float h = cosf(fov / 2.0f) / sinf(fov / 2.0f);
        mat4 projection = {
                aspect * h, 0.0f, 0.0f, 0.0f,
                0.0f, h, 0.0f, 0.0f,
                0.0f, 0.0f, zFar / (zFar - zNear), -(zFar * zNear) / (zFar - zNear),
                0.0f, 0.0f, 1.0f, 0.0f
        };
        //float h = cosf(fov / 2.0f) / sinf(fov / 2.0f);
        //projection = mat4::transpose(projection);
        //projection = mat4::translate(0.0f, 0.0f, 0.0f);

        /*mat4 projection = {
                h / aspect, 0.0f, 0.0f, 0.0f,
                0.0f, h, 0.0f, 0.0f,
                0.0f, 0.0f, zFar / (zFar - zNear), 1.0f,
                0.0f, 0.0f, -zFar * zNear / (zFar - zNear), 0.0f
        };*/

        /*
        mat4 projection = {
                1.0f / (aspect * tanf(fov / 2.0f)), 0.0f, 0.0f, 0.0f,
                0.0f, 1.0f / tanf(fov / 2.0f), 0.0f, 0.0f,
                0.0f, 0.0f, -(zFar + zNear) / (zFar - zNear), -(2.0f * zFar * zNear) / (zFar - zNear),
                0.0f, 0.0f, -1.0f, 0.0f
        };*/
        /*
        mat4 projection = {
                1.0f / (aspect * tanf(fov / 2.0f)), 0.0f, 0.0f, 0.0f,
                0.0f, 1.0f / tanf(fov / 2.0f), 0.0f, 0.0f,
                0.0f, 0.0f, zFar / (zFar - zNear), zFar * zNear / (zNear - zFar),
                0.0f, 0.0f, 1.0f, 0.0f
        };
        /*mat4 projection = {
                tanf(fov / 2.0f) / aspect, 0.0f, 0.0f, 0.0f,
                0.0f, tanf(fov / 2.0f), 0.0f, 0.0f,
                0.0f, 0.0f, -(zFar + zNear) / (zFar - zNear), -(2.0f * zFar * zNear) / (zFar - zNear),
                0.0f, 0.0f, -1.0f, 0.0f
        };*/
        //projection = mat4::transpose(projection);

        /*
        float top = 20.0f;
        float bottom = -20.0f;
        float left = -20.0f;
        float right = 20.0f;
        float far = 100.0f;
        float near = 0.1f;

        /*
        mat4 projection = {
                (right - left) / 2.0f, 0.0f, 0.0f, (left + right) / 2.0f,
                0.0f, (top - bottom) / 2.0f, 0.0f, (top + bottom) / 2.0f,
                0.0f, 0.0f, (far - near) / -2.0f, -(far + near) / 2.0f,
                0.0f, 0.0f, 0.0f, 1.0f
        };*/

        /*
        mat4 projection = {
                2.0f / (right - left), 0.0f, 0.0f, -(right + left) / (right - left),
                0.0f, 2.0f / (top - bottom), 0.0f, -(top + bottom) / (top - bottom),
                0.0f, 0.0f, -2.0f / (far - near), -(far + near) / (far - near),
                0.0f, 0.0f, 0.0f, 1.0f
        };*/


        std::cout << "==================" << std::endl;
        vec3f out = projection * view * model * vec3f(0.0f, 0.0f, 0.0f);
        std::cout << cam.x << "  " << cam.y << "  " << cam.z << std::endl;
        std::cout << out.x << "  " << out.y << "  " << out.z << std::endl;


        //mat4 m1 = mat4::scale(2.0f, 2.0f, 2.0f) * mat4::translate(2.0f, 2.0f, 2.0f) * mat4::scale(0.5f, 0.5f, 0.5f);
        //m1 = mat4::transpose(m1);
        /*mat4 m1;
        std::cout << "------------" << std::endl;
        for(int y = 0; y < 4; y++){
            for(int x = 0; x < 4; x++){
                std::cout << m1[y][x] << " ";
            }
            std::cout << std::endl;
        }*/

        painter::draw(projection * view * model, shaderShadow, {1.0f, 0.0f, 0.0f, 1.0f});

        display::postUpdate();
    }

    // The user has requested the application be closed, so exit
    display::close();
    return 0;
}
