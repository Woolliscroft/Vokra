#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "renderer.h"
#include <stdio.h>

VkState state;

void render() {
    vk_renderRect(&state, 0.1f, 0.2f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f); // green rectangle
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
	(void) window;
	glViewport(0, 0, width, height);
}

int main() {
    if (!glfwInit()) return -1;

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(640, 480, "Rectangle", NULL, NULL);
   
		if (!window) { glfwTerminate(); return -1; }
    glfwMakeContextCurrent(window);
		glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);


    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        fprintf(stderr, "Failed to initialize GLAD\n");
        return -1;
    }

    glViewport(0, 0, 640, 480);

    vk_begin(&state); // initialize renderer

    while (!glfwWindowShouldClose(window)) {
        glClearColor(0.1f, 0.2f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        render(); // draw rectangles

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    vk_end(&state); // cleanup

    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}

