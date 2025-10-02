#ifndef VK_RENDERER_H
#define VK_RENDERER_H

#include <glad/glad.h> 

typedef struct VkState {
    GLuint shaderProgram;
    GLuint vao;
    GLuint vbo;
} VkState;


void vk_begin(VkState* state);
void vk_renderRect(VkState* state, float x, float y, float w, float h, float r, float g, float b);
void vk_end(VkState* state);

#endif

