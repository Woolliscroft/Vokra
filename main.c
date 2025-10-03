#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stdio.h>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include "vokra.h"

VkState state;

// Draw function
void render() {
    vk_renderRect(&state, 400, 300, 200, 100, RED);
    vk_renderETriangle(&state, 200, 150, 100, 100, GREEN);
    // Draw a triangle with explicit absolute vertices
    vk_renderSpTriangle(&state,
        100, 100, // Coords of Triangle 
        300, 100,  // Vertex 2 (etc)
        200, 300,   
        GREY);
	GLuint texID = vk_loadTexture("good.jpg"); 
	vk_displayTexture(&state, texID, 500, 600, 100, 100);
}

// Resize
void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);

    // update state so renderer knows new resolution
    VkState* st = (VkState*)glfwGetWindowUserPointer(window);
    if (st) {
        st->screenWidth  = width;
        st->screenHeight = height;
    }
}

int main() {
    if (!glfwInit()) {
			return -1;
		}
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(640, 480, "Rectangle", NULL, NULL);
    if (!window) { 
			glfwTerminate(); return -1; 
		}

    glfwMakeContextCurrent(window);

    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetWindowUserPointer(window, &state); 

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        fprintf(stderr, "Failed to initialize GLAD\n");
        return -1;
    }
		// Update width + height
    int width, height;
    glfwGetFramebufferSize(window, &width, &height);
    glViewport(0, 0, width, height);

    // store into renderer state // for correct resizing
    state.screenWidth  = width;
    state.screenHeight = height;
		
		// begin rendering
    vk_begin(&state); 

    while (!glfwWindowShouldClose(window)) {
        glClearColor(0.1f, 0.2f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        render(); 

        glfwSwapBuffers(window);
        glfwPollEvents();
    }
		// cleanup
    vk_end(&state); 

    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}

