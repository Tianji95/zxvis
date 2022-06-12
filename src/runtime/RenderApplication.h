#pragma once
#ifndef RENDER_APPLICATION_H
#define RENDER_APPLICATION_H
#ifdef _WIN32
#define VK_USE_PLATFORM_WIN32_KHR // 如果想要用surface，必须要在include vulkan之前添加这句话
#endif
#include <vulkan/vulkan.h>
#include <vector>
#include <string>
#include <assert.h>
#include <Windows.h>
#include "pipelineInfo.h"
constexpr uint32_t APP_NAME_STR_LEN = 80;
constexpr uint32_t WINDOW_WIDTH = 1024;
constexpr uint32_t WINDOW_HEIGHT = 768;
const std::string TEXTURE_PATH = "F:/GitHub/zxvis/resources/images/1.jpg";


class RenderApplication {
public:
    void Run();
private:
    void EnableLayers();
    void EnableExtensions();
    void CreateInstance();
    void CreateDevice();
    void CreateSwapChain();
    void CreateMyWindow(uint32_t width, uint32_t height);
    void CreateCommandPool();
    void CreateVertexBuffer();
    void CreateIndexBuffer();
    void CreateUniformBuffer();
    void CreateRenderPass();
    void CreateDescriptorSetLayout();
    void CreateDescriptorPool();
    void CreateDescriptorSet();
    void CreatePipeline();
    void CreateFrameBuffers();
    void CreateDrawCommandBuffer();
    void CreateFrameSemaphores();
    void CreateTextureImage();
    void CreateTextureImageView();
    void CreateTextureSampler();
    void CreateMsaaImage();

    void DrawFrame();
    void UpdateUniformBuffer();

    VkImageView CreateImageView(VkImage image, VkFormat format);
    void CreateImage(uint32_t width, uint32_t height, VkFormat format, VkImageTiling tiling, VkImageUsageFlags usage, VkMemoryPropertyFlags properties, VkImage& image, VkDeviceMemory& imageMemory, VkSampleCountFlagBits multiSampleCount);;
    VkCommandBuffer CreateSingleTimeCommandBuffer();
    void SubmitSingleTimeCommandBuffer(VkCommandBuffer commandBuffer);
    void TransitionImageLayout(VkImage image, VkFormat format, VkImageLayout oldLayout, VkImageLayout newLayout);
    void CopyBuffer(VkBuffer dstBuffer, VkBuffer srcBuffer, VkDeviceSize size);
    void CopyBufferToImage(VkBuffer buffer, VkImage image, uint32_t width, uint32_t height);
    void CreateBuffer(VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties, VkBuffer& buffer, VkDeviceMemory& bufferMemory);
    uint32_t findMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties);

private:
    std::vector<const char *> enabledLayers;
    std::vector<const char *> enabledExtensions;
    std::vector<const char *> deviceExtensions;

    VkInstance       instance;
    VkDevice         device;
    VkPhysicalDevice physicalDeviceGpu;
    VkQueue          queue;
    uint32_t         queueFamilyIndex;
    std::vector<VkQueueFamilyProperties> queueFamilies;

    HINSTANCE    connection;        // hInstance - Windows Instance
    char         name[APP_NAME_STR_LEN] = "My Draw Triangle"; // Name to put on the window/icon
    HWND         window;                 // hWnd - window handle
    VkSurfaceKHR surface;
    VkFormat     swapChainImageFormat;

    VkSampleCountFlagBits msaaSampleCount = VK_SAMPLE_COUNT_4_BIT;
    VkImage msaaImage;
    VkDeviceMemory msaaImageMemory;
    VkImageView msaaImageView;

    VkSwapchainKHR             swapChain;
    std::vector<VkImage>       swapChainImages;
    std::vector<VkImageView>   swapChainImageViews;
    std::vector<VkFramebuffer> swapChainFramebuffers;
    uint32_t                   swapChainImageCount;
    VkExtent2D                 swapChainExtent;

    VkImage           textureImage;
    VkDeviceMemory    textureImageMemory;
    VkImageView       textureImageView;
    VkSampler         textureSampler;

    VkBuffer          uniformBuffer;
    VkDeviceMemory    uniformBufferMemory;
    VkBuffer          indexBuffer;
    VkDeviceMemory    indexBufferMemory;
    VkBuffer          vertexBuffer;
    VkDeviceMemory    vertexBufferMemory;
    PipelineInfo*     pipelineInfo;
    VkRenderPass      renderPass;

    VkCommandPool   commandPool;
    std::vector<VkCommandBuffer> commandBuffers;

    VkSemaphore imageAvailableSemaphore;
    VkSemaphore renderFinishedSemaphore;

    VkDescriptorSetLayout descriptorSetLayout;
    VkDescriptorPool      descriptorPool;
    VkDescriptorSet       descriptorSet;
};

#endif // ! RENDER_APPLICATION_H

