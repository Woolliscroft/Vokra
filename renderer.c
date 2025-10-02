#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stdio.h>

#include "renderer.h"

struct VkState{
  unsigned int shaderProgram;
};

void vk_begin(VKState* state){}
void vk_renderRect(VKState*, float x, float y, float w, float h){}
void vk_end(VkState* state) {}
