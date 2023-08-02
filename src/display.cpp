
#include <map>

#include <GL/glew.h>
#include <glfw/glfw3.h>

#include "geo/vec2f.h"
#include "geo/vec2i.h"
#include "display.h"

static bool eventMousePress, eventMouseRelease;
static std::map<int, bool> keyStatesLast;

// GLFW callback that notifies us on mouse button events
void callbackMouseButton(GLFWwindow* window, int button, int action, int mods) {
    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) eventMousePress = true;
    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_RELEASE) eventMouseRelease = true;
}

namespace display {

    static GLFWwindow* window;
    static vec2i windowSize;
    static vec2f locationCursor;

    void initialize() {
        // TODO make the window resolution change based on monitor resolution
        windowSize = {1920, 1080};

        // Initialize the GLFW fullscreen window
        glfwInit();
        glfwDefaultWindowHints();
        glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
        window = glfwCreateWindow(windowSize.x, windowSize.y, "Shadow Mapping Demo", glfwGetPrimaryMonitor(), nullptr);
        glfwMakeContextCurrent(window);

        // Listen for mouse button events
        glfwSetMouseButtonCallback(window, callbackMouseButton);

        glewInit();
    }

    void preUpdate() {
        eventMousePress = false;
        eventMouseRelease = false;

        glfwPollEvents();

        // Update mouse position
        double x, y;
        glfwGetCursorPos(window, &x, &y);
        locationCursor.x = (float)x;
        locationCursor.y = (float)y;

        // Prepare the OpenGL context for rendering
        glViewport(0, 0, windowSize.x, windowSize.y);
        glClear(GL_COLOR_BUFFER_BIT);
    }

    void postUpdate() {
        glfwSwapBuffers(window);

        // Update key values based on data from this tick
        for (const auto& keyState : keyStatesLast) {
            keyStatesLast[keyState.first] = glfwGetKey(window, keyState.first) == GLFW_PRESS;
        }
    }

    void close() {
        glfwTerminate();
    }

    bool isExiting() {
        // Clicking the window close button or pressing the escape key will request a program exit
        return glfwWindowShouldClose(window) || glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS;
    }

    const vec2i& getSize() {
        return windowSize;
    }

    const vec2f& getCursor() {
        return locationCursor;
    }

    bool hasEventMousePress() {
        return eventMousePress;
    }

    bool hasEventMouseRelease() {
        return eventMouseRelease;
    }

    bool hasEventKeyPress(int key) {
        if(!keyStatesLast.count(key)) { // Check if the polled key is being tracked, if not add it to states list
            keyStatesLast[key] = glfwGetKey(window, key) == GLFW_PRESS;
            return false;
        } else return glfwGetKey(window, key) == GLFW_PRESS && keyStatesLast.count(key) && !keyStatesLast[key];
    }

    bool hasEventKeyRelease(int key) {
        if(!keyStatesLast.count(key)) { // Check if the polled key is being tracked, if not add it to states list
            keyStatesLast[key] = glfwGetKey(window, key) == GLFW_PRESS;
            return false;
        } else return glfwGetKey(window, key) == GLFW_RELEASE && keyStatesLast.count(key) && keyStatesLast[key];
    }

}
