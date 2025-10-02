#ifndef VK_RENDERER_H
#define VK_RENDERER_H

#include <glad/glad.h> 

typedef struct VkState {
    GLuint shaderProgram;
    GLuint vao;
    GLuint vbo;
} VkState;

typedef struct Colour {
    float r, g, b;
} Colour;

// Named colors as constants
static const Colour RED   = {1.0f, 0.0f, 0.0f};
static const Colour GREEN = {0.0f, 1.0f, 0.0f};
static const Colour BLUE  = {0.0f, 0.0f, 1.0f};
static const Colour WHITE = {1.0f, 1.0f, 1.0f};
static const Colour BLACK = {0.0f, 0.0f, 0.0f};


void vk_begin(VkState* state);
void vk_renderRect(VkState* state, float x, float y, float w, float h, Colour color);
void vk_renderTriangle(VkState* state, float x, float y, float w, float h, Colour color);
void vk_end(VkState* state);

#endif

