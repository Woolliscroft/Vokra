#indef VK_RENDERER_H
#define VK_RENDERER_H

typedef struct VkState VkState; // pass shader data
void vk_begin(VKState* state);
void vk_renderRect(VKState*, float x, float y, float w, float h);
void vk_end(VkState* state);
#endif
