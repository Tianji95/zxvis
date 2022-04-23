#pragma once
#ifndef _COMMON_H_
#define _COMMON_H_
#include <vulkan/vulkan.h>
#include "vec.h"

struct UniformBufferObject {
    mat4 model;
    mat4 view;
    mat4 proj;
};

void inline VK_CHECK_RESULT(const VkResult& f)
{
    VkResult res = (f);
    if (res != VK_SUCCESS) {
        printf("Fatal : VkResult is %d in %s at line %d\n", res, __FILE__, __LINE__);
        assert(res == VK_SUCCESS);
    }
}
uint32_t* readFile(uint32_t& length, const char* filename);

#endif // !_COMMON_H_

