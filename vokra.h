#ifndef VK_RENDERER_H
#define VK_RENDERER_H

#include <glad/glad.h>

typedef struct VkState {
    GLuint shaderProgram;
    GLuint vao;
    GLuint vbo;
		int screenWidth;
    int screenHeight;
} VkState;

typedef struct Colour {
    float r, g, b;
} Colour;

typedef struct {
	unsigned char *data;
	int width;
	int height;
	int channels;
}	Texture;

// Colours
static const Colour RED   = {1.0f, 0.0f, 0.0f};
static const Colour GREEN = {0.0f, 1.0f, 0.0f};
static const Colour BLUE  = {0.0f, 0.0f, 1.0f};
static const Colour WHITE = {1.0f, 1.0f, 1.0f};
static const Colour BLACK = {0.0f, 0.0f, 0.0f};
static const Colour YELLOW  = {1.0f, 1.0f, 0.0f};
static const Colour CYAN    = {0.0f, 1.0f, 1.0f};
static const Colour MAGENTA = {1.0f, 0.0f, 1.0f};
static const Colour ORANGE  = {1.0f, 0.5f, 0.0f};
static const Colour PURPLE  = {0.5f, 0.0f, 0.5f};
static const Colour GREY    = {0.5f, 0.5f, 0.5f};
static const Colour BROWN   = {0.6f, 0.3f, 0.0f};
static const Colour PINK    = {1.0f, 0.75f, 0.8f};
static const Colour LIME    = {0.75f, 1.0f, 0.0f};
static const Colour TEAL    = {0.0f, 0.5f, 0.5f};

// === Shape Rendering using OpenGL === //
void vk_begin(VkState* state);
// Drawn from Bottom Left.
void vk_renderRect(VkState* state, float screenX, float screenY, float width, float height, Colour color);
void vk_renderETriangle(VkState* state, float screenX, float screenY, float widht, float height, Colour color);
void vk_renderSpTriangle(VkState* state, float x1, float y1, float x2, float y2, float x3, float y3, Colour color);
// Deletes State
void vk_end(VkState* state);

// === Texture Loading using stb_image.h === //

// Texture vk_loadTexture(const char *filepath);
GLuint vk_loadTexture(const char *filepath);
void vk_textureDispose(Texture *tex);
void vk_displayTexture(VkState *state, GLuint texID, float x, float y, float w, float h);

#endif

