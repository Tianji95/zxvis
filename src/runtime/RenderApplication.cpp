#include "RenderApplication.h"
#include <iostream>
#include <algorithm>
#include <array>
#include <glm/gtc/matrix_transform.hpp>
#define STB_IMAGE_IMPLEMENTATION // 这个算是stb的一个小坑，必须要先定义宏，才能用stb的实现
#include <stb/stb_image.h>
#include <chrono>
#include "verticesData.h"

void RenderApplication::Run()
{
    EnableLayers();
    EnableExtensions();
    CreateInstance();
    CreateDevice();
    CreateMyWindow(WINDOW_WIDTH, WINDOW_HEIGHT);
    CreateSwapChain();
    CreateCommandPool();
    //CreateTextureImage();
    //CreateTextureImageView();
    //CreateTextureSampler();
    CreateVertexBuffer();
    CreateIndexBuffer();
    CreateUniformBuffer();
    CreateRenderPass();
    CreateDescriptorSetLayout();
    CreateDescriptorPool();
    CreateDescriptorSet();
    CreatePipeline();
    CreateFrameBuffers();
    CreateDrawCommandBuffer();
    CreateFrameSemaphores();

    while (true) {
        //UpdateUniformBuffer();
        DrawFrame();
    }

    vkDestroySemaphore(device, renderFinishedSemaphore, nullptr);
    vkDestroySemaphore(device, imageAvailableSemaphore, nullptr);
    vkFreeCommandBuffers(device, commandPool, 2, commandBuffers.data());
    vkDestroyCommandPool(device, commandPool, nullptr);
    for (size_t i = 0; i < swapChainFramebuffers.size(); i++) {
        vkDestroyFramebuffer(device, swapChainFramebuffers[i], nullptr);
    }
    for (uint32_t i = 0; i < swapChainImageCount; i++) {
        vkDestroyImageView(device, swapChainImageViews[i], nullptr);
    }

    vkDestroyPipeline(device, pipeline, nullptr);
    vkDestroyPipelineLayout(device, pipelineLayout, nullptr);
    vkDestroyDescriptorPool(device, descriptorPool, nullptr);
    vkDestroyDescriptorSetLayout(device, descriptorSetLayout, nullptr);
    //vkDestroyImage(device, textureImage, nullptr);
    //vkFreeMemory(device, textureImageMemory, nullptr);
    vkDestroyBuffer(device, uniformBuffer, nullptr);
    vkFreeMemory(device, uniformBufferMemory, nullptr);
    vkDestroyRenderPass(device, renderPass, nullptr);
    vkDestroyBuffer(device, indexBuffer, nullptr);
    vkFreeMemory(device, indexBufferMemory, nullptr);
    vkDestroyBuffer(device, vertexBuffer, nullptr);
    vkFreeMemory(device, vertexBufferMemory, nullptr);
    //vkDestroySampler(device, textureSampler, nullptr);
    //vkDestroyImageView(device, textureImageView, nullptr);
    vkDestroySwapchainKHR(device, swapChain, nullptr);
    vkDestroySurfaceKHR(instance, surface, nullptr);
    DestroyWindow(window);
    vkDestroyDevice(device, nullptr);
    vkDestroyInstance(instance, nullptr);
}

void RenderApplication::EnableLayers()
{
    uint32_t layerCount;
    vkEnumerateInstanceLayerProperties(&layerCount, NULL);
    std::vector<VkLayerProperties> layerProperties(layerCount);
    vkEnumerateInstanceLayerProperties(&layerCount, layerProperties.data());

    bool foundLayer = false;
    for (VkLayerProperties prop : layerProperties) {
        if (strcmp("VK_LAYER_LUNARG_standard_validation", prop.layerName) == 0) {
            enabledLayers.push_back("VK_LAYER_LUNARG_standard_validation"); // Alright, we can use this layer.
            foundLayer = true;
            break;
        }
    }

    //if (!foundLayer) {
    //    throw std::runtime_error("Layer VK_LAYER_LUNARG_standard_validation not supported\n");
    //}
}

void RenderApplication::EnableExtensions()
{
    bool foundExtension = false;
    uint32_t extensionCount;
    vkEnumerateInstanceExtensionProperties(NULL, &extensionCount, NULL);
    std::vector<VkExtensionProperties> extensionProperties(extensionCount);
    vkEnumerateInstanceExtensionProperties(NULL, &extensionCount, extensionProperties.data());

    for (VkExtensionProperties prop : extensionProperties) {
        if (strcmp(VK_EXT_DEBUG_REPORT_EXTENSION_NAME, prop.extensionName) == 0) {
            enabledExtensions.push_back(VK_EXT_DEBUG_REPORT_EXTENSION_NAME);
            foundExtension = true;
            break;
        }
    }
    enabledExtensions.push_back(VK_KHR_SURFACE_EXTENSION_NAME);//注意，如果用到了显示的xxxKHR，这里一定要加上，如果不加的话会报VK_ERROR_EXTENSION_NOT_PRESENT错误
    enabledExtensions.push_back(VK_KHR_WIN32_SURFACE_EXTENSION_NAME);

    //if (!foundExtension) {
    //    throw std::runtime_error("Extension VK_EXT_DEBUG_REPORT_EXTENSION_NAME not supported\n");
    //}
}

void RenderApplication::CreateInstance()
{
    VkApplicationInfo appinfo = {};
    appinfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
    appinfo.pNext = NULL;
    appinfo.pApplicationName = "Triangle_demo";
    appinfo.applicationVersion = 1;
    appinfo.pEngineName = "NO_ENGINE";
    appinfo.engineVersion = 1;
    appinfo.apiVersion = VK_API_VERSION_1_0;

    VkInstanceCreateInfo createInfo = {};
    createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
    createInfo.pNext = NULL;
    createInfo.flags = 0;
    createInfo.pApplicationInfo = &appinfo;
    createInfo.enabledLayerCount = enabledLayers.size();
    createInfo.ppEnabledLayerNames = enabledLayers.data();
    createInfo.enabledExtensionCount = enabledExtensions.size();
    createInfo.ppEnabledExtensionNames = enabledExtensions.data();

    VkResult res;
    res = vkCreateInstance(&createInfo, NULL, &instance);

}

void RenderApplication::CreateDevice()
{
    // 查找physicalDeviceGpu
    uint32_t deviceCount = 0;
    vkEnumeratePhysicalDevices(instance, &deviceCount, NULL);
    if (deviceCount == 0) {
        throw std::runtime_error("could not find a device with vulkan support");
    }

    std::vector<VkPhysicalDevice> devices(deviceCount);
    vkEnumeratePhysicalDevices(instance, &deviceCount, devices.data());
    for (VkPhysicalDevice device : devices) {
        physicalDeviceGpu = device;
        break;
    }

    // 查找可以支持渲染的queue family info
    uint32_t queueFamilyCount = 0;
    queueFamilyIndex = queueFamilyCount;
    vkGetPhysicalDeviceQueueFamilyProperties(physicalDeviceGpu, &queueFamilyCount, NULL);
    queueFamilies.resize(queueFamilyCount);
    vkGetPhysicalDeviceQueueFamilyProperties(physicalDeviceGpu, &queueFamilyCount, queueFamilies.data());
    for (uint32_t i = 0; i < queueFamilies.size(); i++) {
        VkQueueFamilyProperties props = queueFamilies[i];
        if (props.queueCount > 0 && (props.queueFlags & VK_QUEUE_GRAPHICS_BIT)) {
            queueFamilyIndex = i;
            break;
        }
    }
    assert(queueFamilyIndex != queueFamilyCount);

    deviceExtensions.push_back(VK_KHR_SWAPCHAIN_EXTENSION_NAME);

    // 创建logical device
    float queue_priorities[]{ 1.0f };
    VkDeviceQueueCreateInfo queueCreateInfo = {};
    queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
    queueCreateInfo.queueFamilyIndex = queueFamilyIndex;
    queueCreateInfo.queueCount = 1;
    queueCreateInfo.pQueuePriorities = queue_priorities;

    VkPhysicalDeviceFeatures deviceFeatures = {};
    deviceFeatures.samplerAnisotropy = VK_TRUE;

    VkDeviceCreateInfo deviceCreateInfo = {};
    deviceCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
    deviceCreateInfo.queueCreateInfoCount = 1;
    deviceCreateInfo.pQueueCreateInfos = &queueCreateInfo;
    deviceCreateInfo.enabledLayerCount = enabledLayers.size();
    deviceCreateInfo.ppEnabledLayerNames = enabledLayers.data();
    deviceCreateInfo.enabledExtensionCount = deviceExtensions.size();   //踩坑，instance支持extensions并不代表设备支持extensions，这里加上会报错
    deviceCreateInfo.ppEnabledExtensionNames = deviceExtensions.data();
    deviceCreateInfo.pEnabledFeatures = &deviceFeatures;
    VK_CHECK_RESULT(vkCreateDevice(physicalDeviceGpu, &deviceCreateInfo, NULL, &device)); // create logical device.

    vkGetDeviceQueue(device, queueFamilyIndex, 0, &queue);
}

void RenderApplication::CreateSwapChain()
{
    uint32_t ReqExtensionCount = 0;

    VkWin32SurfaceCreateInfoKHR surfaceCreateInfo = {};
    surfaceCreateInfo.sType = VK_STRUCTURE_TYPE_WIN32_SURFACE_CREATE_INFO_KHR;
    surfaceCreateInfo.pNext = NULL;
    surfaceCreateInfo.hinstance = connection;
    surfaceCreateInfo.hwnd = window;
    VK_CHECK_RESULT(vkCreateWin32SurfaceKHR(instance, &surfaceCreateInfo, NULL, &surface)); //注意，此处一定要在最前面添加extensions，如果不添加的话，这里会报VK_ERROR_EXTENSION_NOT_PRESENT

    uint32_t formatCount = 0;
    VK_CHECK_RESULT(vkGetPhysicalDeviceSurfaceFormatsKHR(physicalDeviceGpu, surface, &formatCount, NULL));
    std::vector<VkSurfaceFormatKHR> surfFormats(formatCount);
    VK_CHECK_RESULT(vkGetPhysicalDeviceSurfaceFormatsKHR(physicalDeviceGpu, surface, &formatCount, surfFormats.data()));
    assert(formatCount >= 1);
    if (formatCount == 1 && surfFormats[0].format == VK_FORMAT_UNDEFINED) {
        swapChainImageFormat = VK_FORMAT_B8G8R8A8_UNORM;
    }
    else {
        swapChainImageFormat = surfFormats[0].format;
    }

    VkSurfaceCapabilitiesKHR surfCapabilities;
    VK_CHECK_RESULT(vkGetPhysicalDeviceSurfaceCapabilitiesKHR(physicalDeviceGpu, surface, &surfCapabilities));

    if (surfCapabilities.currentExtent.width == 0xFFFFFFFF) {
        swapChainExtent.width = std::min(std::max(WINDOW_WIDTH, surfCapabilities.minImageExtent.width), surfCapabilities.maxImageExtent.width);
        swapChainExtent.height = std::min(std::max(WINDOW_HEIGHT, surfCapabilities.minImageExtent.height), surfCapabilities.maxImageExtent.height);
    }
    else {
        swapChainExtent = surfCapabilities.currentExtent;
    }

    VkSurfaceTransformFlagBitsKHR preTransform;
    if (surfCapabilities.supportedTransforms & VK_SURFACE_TRANSFORM_IDENTITY_BIT_KHR) {
        preTransform = VK_SURFACE_TRANSFORM_IDENTITY_BIT_KHR;
    }
    else {
        preTransform = surfCapabilities.currentTransform;
    }
    // Find a supported composite alpha mode - one of these is guaranteed to be set
    VkCompositeAlphaFlagBitsKHR compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
    VkCompositeAlphaFlagBitsKHR compositeAlphaFlags[4] = {
        VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR,
        VK_COMPOSITE_ALPHA_PRE_MULTIPLIED_BIT_KHR,
        VK_COMPOSITE_ALPHA_POST_MULTIPLIED_BIT_KHR,
        VK_COMPOSITE_ALPHA_INHERIT_BIT_KHR,
    };
    for (uint32_t i = 0; i < sizeof(compositeAlphaFlags) / sizeof(compositeAlphaFlags[0]); i++) {
        if (surfCapabilities.supportedCompositeAlpha & compositeAlphaFlags[i]) {
            compositeAlpha = compositeAlphaFlags[i];
            break;
        }
    }

    // Iterate over each queue to learn whether it supports presenting:
    VkBool32 *pSupportsPresent = (VkBool32 *)malloc(queueFamilies.size() * sizeof(VkBool32));
    for (uint32_t i = 0; i < queueFamilies.size(); i++) {
        vkGetPhysicalDeviceSurfaceSupportKHR(physicalDeviceGpu, i, surface, &pSupportsPresent[i]);
    }

    // Search for a graphics and a present queue in the array of queue
    // families, try to find one that supports both
    uint32_t graphics_queue_family_index = UINT32_MAX;
    uint32_t present_queue_family_index = UINT32_MAX;
    for (uint32_t i = 0; i < queueFamilies.size(); ++i) {
        if ((queueFamilies[i].queueFlags & VK_QUEUE_GRAPHICS_BIT) != 0) {
            if (graphics_queue_family_index == UINT32_MAX) graphics_queue_family_index = i;

            if (pSupportsPresent[i] == VK_TRUE) {
                graphics_queue_family_index = i;
                present_queue_family_index = i;
                break;
            }
        }
    }

    if (present_queue_family_index == UINT32_MAX) {
        // If didn't find a queue that supports both graphics and present, then
        // find a separate present queue.
        for (size_t i = 0; i < queueFamilies.size(); ++i)
            if (pSupportsPresent[i] == VK_TRUE) {
                present_queue_family_index = i;
                break;
            }
    }
    free(pSupportsPresent);

    VkSwapchainCreateInfoKHR swapChainCreateInfo = {};
    swapChainCreateInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
    swapChainCreateInfo.surface = surface;
    swapChainCreateInfo.minImageCount = surfCapabilities.minImageCount;
    swapChainCreateInfo.imageFormat = swapChainImageFormat;
    swapChainCreateInfo.imageColorSpace = VK_COLOR_SPACE_SRGB_NONLINEAR_KHR;
    swapChainCreateInfo.imageExtent = swapChainExtent;
    swapChainCreateInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;
    swapChainCreateInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
    swapChainCreateInfo.imageArrayLayers = 1;
    swapChainCreateInfo.compositeAlpha = compositeAlpha;
    swapChainCreateInfo.preTransform = preTransform;
    swapChainCreateInfo.presentMode = VK_PRESENT_MODE_FIFO_KHR;
    swapChainCreateInfo.oldSwapchain = VK_NULL_HANDLE;
    swapChainCreateInfo.clipped = VK_TRUE;
    uint32_t queueFamilyIndices[2] = { (uint32_t)graphics_queue_family_index, (uint32_t)present_queue_family_index };
    if (graphics_queue_family_index != present_queue_family_index) {
        // If the graphics and present queues are from different queue families,
        // we either have to explicitly transfer ownership of images between
        // the queues, or we have to create the swapchain with imageSharingMode
        // as VK_SHARING_MODE_CONCURRENT
        swapChainCreateInfo.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
        swapChainCreateInfo.queueFamilyIndexCount = 2;
        swapChainCreateInfo.pQueueFamilyIndices = queueFamilyIndices;
    }

    VK_CHECK_RESULT(vkCreateSwapchainKHR(device, &swapChainCreateInfo, NULL, &swapChain)); // 踩坑，想要用这个KHR，也必须在前面的device extension里面添加swapchain的extension

    vkGetSwapchainImagesKHR(device, swapChain, &swapChainImageCount, nullptr);
    swapChainImages.resize(swapChainImageCount);
    vkGetSwapchainImagesKHR(device, swapChain, &swapChainImageCount, swapChainImages.data());
    swapChainImageViews.resize(swapChainImageCount);

    for (uint32_t i = 0; i < swapChainImageCount; i++) {
        swapChainImageViews[i] = CreateImageView(swapChainImages[i], swapChainImageFormat);
    }
}

// MS-Windows event handling function:
LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch (uMsg) {
    case WM_CLOSE:
        PostQuitMessage(0);
        break;
    case WM_PAINT:
        return 0;
    default:
        break;
    }
    return (DefWindowProc(hWnd, uMsg, wParam, lParam));
}

void RenderApplication::CreateMyWindow(uint32_t width, uint32_t height)
{
    WNDCLASSEX win_class;
    assert(width > 0);
    assert(height > 0);

    connection = GetModuleHandle(NULL);

    // Initialize the window class structure:
    win_class.cbSize = sizeof(WNDCLASSEX);
    win_class.style = CS_HREDRAW | CS_VREDRAW;
    win_class.lpfnWndProc = WndProc;
    win_class.cbClsExtra = 0;
    win_class.cbWndExtra = 0;
    win_class.hInstance = connection;  // hInstance
    win_class.hIcon = LoadIcon(NULL, IDI_APPLICATION);
    win_class.hCursor = LoadCursor(NULL, IDC_ARROW);
    win_class.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
    win_class.lpszMenuName = NULL;
    win_class.lpszClassName = name;
    win_class.hIconSm = LoadIcon(NULL, IDI_WINLOGO);
    // Register window class:
    if (!RegisterClassEx(&win_class)) {
        // It didn't work, so try to give a useful error:
        printf("Unexpected error trying to start the application!\n");
        fflush(stdout);
        exit(1);
    }
    // Create window with the registered class:
    RECT wr = { 0, 0, width, height };
    AdjustWindowRect(&wr, WS_OVERLAPPEDWINDOW, FALSE);
    window = CreateWindowEx(0,
        name,             // class name
        name,             // app name
        WS_OVERLAPPEDWINDOW |  // window style
        WS_VISIBLE | WS_SYSMENU,
        100, 100,            // x/y coords
        wr.right - wr.left,  // width
        wr.bottom - wr.top,  // height
        NULL,                // handle to parent
        NULL,                // handle to menu
        connection,     // hInstance
        NULL);               // no extra parameters
    if (!window) {
        // It didn't work, so try to give a useful error:
        printf("Cannot create a window in which to draw!\n");
        fflush(stdout);
        exit(1);
    }
    //SetWindowLongPtr(window, GWLP_USERDATA, (LONG_PTR)&info);
}

uint32_t* RenderApplication::readFile(uint32_t& length, const char* filename)
{
    FILE* fp;
    fopen_s(&fp, filename, "rb");
    if (fp == NULL) {
        printf("Could not find or open file: %s\n", filename);
    }

    // get file size.
    fseek(fp, 0, SEEK_END);
    long filesize = ftell(fp);
    fseek(fp, 0, SEEK_SET);

    long filesizepadded = long(ceil(filesize / 4.0)) * 4;

    // read file contents.
    char *str = new char[filesizepadded];
    fread(str, filesize, sizeof(char), fp);
    fclose(fp);

    // data padding. 
    for (int i = filesize; i < filesizepadded; i++) {
        str[i] = 0;
    }

    length = filesizepadded;
    return (uint32_t *)str;
}

void RenderApplication::CreateCommandPool()
{
    VkCommandPoolCreateInfo poolInfo = {};
    poolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
    poolInfo.queueFamilyIndex = queueFamilyIndex;
    poolInfo.flags = 0; // Optional
    VK_CHECK_RESULT(vkCreateCommandPool(device, &poolInfo, nullptr, &commandPool));
}

uint32_t RenderApplication::findMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties)
{
    VkPhysicalDeviceMemoryProperties memProperties;
    vkGetPhysicalDeviceMemoryProperties(physicalDeviceGpu, &memProperties);
    for (uint32_t i = 0; i < memProperties.memoryTypeCount; i++) {
        if ((typeFilter & (1 << i)) && (memProperties.memoryTypes[i].propertyFlags & properties) == properties) {
            return i;
        }
    }
}

void RenderApplication::CopyBuffer(VkBuffer dstBuffer, VkBuffer srcBuffer, VkDeviceSize size)
{
    VkCommandBuffer copyMemoryCommandBuffer = CreateSingleTimeCommandBuffer();

    VkBufferCopy copyRegion = {};
    copyRegion.srcOffset = 0; // Optional
    copyRegion.dstOffset = 0; // Optional
    copyRegion.size = size;
    vkCmdCopyBuffer(copyMemoryCommandBuffer, srcBuffer, dstBuffer, 1, &copyRegion);

    SubmitSingleTimeCommandBuffer(copyMemoryCommandBuffer);
}

void RenderApplication::CreateBuffer(VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties, VkBuffer& buffer, VkDeviceMemory& bufferMemory)
{
    VkBufferCreateInfo bufferInfo = {};
    bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
    bufferInfo.size = size;
    bufferInfo.usage = usage;
    bufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
    VK_CHECK_RESULT(vkCreateBuffer(device, &bufferInfo, nullptr, &buffer));

    VkMemoryRequirements memRequirements;
    vkGetBufferMemoryRequirements(device, buffer, &memRequirements);

    VkMemoryAllocateInfo allocInfo = {};
    allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
    allocInfo.allocationSize = memRequirements.size;
    allocInfo.memoryTypeIndex = findMemoryType(memRequirements.memoryTypeBits, properties);
    VK_CHECK_RESULT(vkAllocateMemory(device, &allocInfo, nullptr, &bufferMemory));

    vkBindBufferMemory(device, buffer, bufferMemory, 0);
}

void RenderApplication::CreateVertexBuffer()
{
    VkDeviceSize bufferSize = sizeof(vertices[0]) * vertices.size();
    VkBuffer stagingBuffer;
    VkDeviceMemory stagingBufferMemory;

    CreateBuffer(bufferSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, stagingBuffer, stagingBufferMemory);
    void* data;
    vkMapMemory(device, stagingBufferMemory, 0, bufferSize, 0, &data);
    memcpy(data, vertices.data(), (size_t)bufferSize);
    vkUnmapMemory(device, stagingBufferMemory);

    CreateBuffer(bufferSize, VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, vertexBuffer, vertexBufferMemory);
    CopyBuffer(vertexBuffer, stagingBuffer, bufferSize);

    vkDestroyBuffer(device, stagingBuffer, nullptr);
    vkFreeMemory(device, stagingBufferMemory, nullptr);
}

void RenderApplication::CreateIndexBuffer()
{
    VkDeviceSize bufferSize = sizeof(indices[0]) * indices.size();

    VkBuffer stagingBuffer;
    VkDeviceMemory stagingBufferMemory;
    CreateBuffer(bufferSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, stagingBuffer, stagingBufferMemory);

    void* data;
    vkMapMemory(device, stagingBufferMemory, 0, bufferSize, 0, &data);
    memcpy(data, indices.data(), (size_t)bufferSize);
    vkUnmapMemory(device, stagingBufferMemory);

    CreateBuffer(bufferSize, VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_INDEX_BUFFER_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, indexBuffer, indexBufferMemory);

    CopyBuffer(indexBuffer, stagingBuffer, bufferSize);

    vkDestroyBuffer(device, stagingBuffer, nullptr);
    vkFreeMemory(device, stagingBufferMemory, nullptr);
}

void RenderApplication::CreateUniformBuffer()
{
    VkDeviceSize bufferSize = sizeof(UniformBufferObject);
    CreateBuffer(bufferSize, VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, uniformBuffer, uniformBufferMemory);

    UniformBufferObject ubo;
    ubo.model = glm::mat4(1.0f);
    ubo.view = glm::lookAt(glm::vec3(0.0f, 0.0f, -2.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    ubo.proj = glm::perspective(glm::radians(45.0f), swapChainExtent.width / (float)swapChainExtent.height, 0.1f, 10.0f);
    ubo.proj[1][1] *= -1;

    void* data;
    vkMapMemory(device, uniformBufferMemory, 0, sizeof(ubo), 0, &data);
    memcpy(data, &ubo, sizeof(ubo));
    vkUnmapMemory(device, uniformBufferMemory);
}

void RenderApplication::CreateRenderPass()
{
    VkAttachmentDescription colorAttachment = {};
    colorAttachment.format = swapChainImageFormat;
    colorAttachment.samples = VK_SAMPLE_COUNT_1_BIT;
    colorAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
    colorAttachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
    colorAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
    colorAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
    colorAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
    colorAttachment.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;

    VkAttachmentReference colorAttachmentRef = {};
    colorAttachmentRef.attachment = 0;
    colorAttachmentRef.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

    VkSubpassDescription subpass = {};
    subpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
    subpass.colorAttachmentCount = 1;
    subpass.pColorAttachments = &colorAttachmentRef;

    VkRenderPassCreateInfo renderPassInfo = {};
    renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
    renderPassInfo.attachmentCount = 1;
    renderPassInfo.pAttachments = &colorAttachment;
    renderPassInfo.subpassCount = 1;
    renderPassInfo.pSubpasses = &subpass;

    VK_CHECK_RESULT(vkCreateRenderPass(device, &renderPassInfo, nullptr, &renderPass));
}

void RenderApplication::CreateDescriptorSetLayout()
{
    //VkDescriptorSetLayoutBinding samplerLayoutBindingCI = {};
    //samplerLayoutBindingCI.binding = 1;
    //samplerLayoutBindingCI.descriptorCount = 1;
    //samplerLayoutBindingCI.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
    //samplerLayoutBindingCI.pImmutableSamplers = nullptr;
    //samplerLayoutBindingCI.stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT;

    VkDescriptorSetLayoutBinding uboLayoutBindingCI = {};
    uboLayoutBindingCI.binding = 0;
    uboLayoutBindingCI.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
    uboLayoutBindingCI.descriptorCount = 1;
    uboLayoutBindingCI.stageFlags = VK_SHADER_STAGE_VERTEX_BIT;
    uboLayoutBindingCI.pImmutableSamplers = nullptr;

    std::array<VkDescriptorSetLayoutBinding,1> bindings = { uboLayoutBindingCI };
    //std::array<VkDescriptorSetLayoutBinding, 2> bindings = { uboLayoutBindingCI, samplerLayoutBindingCI };

    VkDescriptorSetLayoutCreateInfo descriptorSetLayoutCI = {};
    descriptorSetLayoutCI.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
    descriptorSetLayoutCI.bindingCount = static_cast<uint32_t>(bindings.size());
    descriptorSetLayoutCI.pBindings = bindings.data();

    VK_CHECK_RESULT(vkCreateDescriptorSetLayout(device, &descriptorSetLayoutCI, nullptr, &descriptorSetLayout));
}

void RenderApplication::CreateDescriptorPool()
{
    std::array<VkDescriptorPoolSize, 2> poolSizes = {};
    poolSizes[0].type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
    poolSizes[0].descriptorCount = 1;
    //poolSizes[1].type = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
    //poolSizes[1].descriptorCount = 1;

    VkDescriptorPoolCreateInfo poolInfo = {};
    poolInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
    poolInfo.poolSizeCount = static_cast<uint32_t>(poolSizes.size());
    poolInfo.pPoolSizes = poolSizes.data();
    poolInfo.maxSets = 1;
    VK_CHECK_RESULT(vkCreateDescriptorPool(device, &poolInfo, nullptr, &descriptorPool));
}

void RenderApplication::CreateDescriptorSet()
{
    VkDescriptorSetLayout layouts[] = { descriptorSetLayout };
    VkDescriptorSetAllocateInfo allocInfo = {};
    allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
    allocInfo.descriptorPool = descriptorPool;
    allocInfo.descriptorSetCount = 1;
    allocInfo.pSetLayouts = layouts;
    VK_CHECK_RESULT(vkAllocateDescriptorSets(device, &allocInfo, &descriptorSet));

    VkDescriptorBufferInfo bufferInfo = {};
    bufferInfo.buffer = uniformBuffer;
    bufferInfo.offset = 0;
    bufferInfo.range = sizeof(UniformBufferObject);
    std::array<VkWriteDescriptorSet, 1> descriptorWrites = {};
    //std::array<VkWriteDescriptorSet, 2> descriptorWrites = {};
    descriptorWrites[0].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
    descriptorWrites[0].dstSet = descriptorSet;
    descriptorWrites[0].dstBinding = 0;
    descriptorWrites[0].dstArrayElement = 0;
    descriptorWrites[0].descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
    descriptorWrites[0].descriptorCount = 1;
    descriptorWrites[0].pBufferInfo = &bufferInfo;
    descriptorWrites[0].pImageInfo = nullptr; // Optional
    descriptorWrites[0].pTexelBufferView = nullptr; // Optiona

    //VkDescriptorImageInfo imageInfo = {};
    //imageInfo.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
    //imageInfo.imageView = textureImageView;
    //imageInfo.sampler = textureSampler;

    //descriptorWrites[1].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
    //descriptorWrites[1].dstSet = descriptorSet;
    //descriptorWrites[1].dstBinding = 1;
    //descriptorWrites[1].dstArrayElement = 0;
    //descriptorWrites[1].descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
    //descriptorWrites[1].descriptorCount = 1;
    //descriptorWrites[1].pImageInfo = &imageInfo;

    vkUpdateDescriptorSets(device, static_cast<uint32_t>(descriptorWrites.size()), descriptorWrites.data(), 0, nullptr);
}

void RenderApplication::CreatePipeline()
{
    uint32_t filelength;
    uint32_t* vertexShaderCode = readFile(filelength, "F:/GitHub/zxvis/src/shaders/texture.vert.spv");		// tmp
    VkShaderModuleCreateInfo vertexShaderCreateInfo = {};
    vertexShaderCreateInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
    vertexShaderCreateInfo.pCode = vertexShaderCode;
    vertexShaderCreateInfo.codeSize = filelength;
    VK_CHECK_RESULT(vkCreateShaderModule(device, &vertexShaderCreateInfo, NULL, &vertexShaderModule));
    delete[] vertexShaderCode;

    uint32_t* fragmentShaderCode = readFile(filelength, "F:/GitHub/zxvis/src/shaders/texture.frag.spv");		// tmp
    VkShaderModuleCreateInfo fragmentShaderCreateInfo = {};
    fragmentShaderCreateInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
    fragmentShaderCreateInfo.pCode = fragmentShaderCode;
    fragmentShaderCreateInfo.codeSize = filelength;
    VK_CHECK_RESULT(vkCreateShaderModule(device, &fragmentShaderCreateInfo, NULL, &fragmentShaderModule));
    delete[] fragmentShaderCode;

    std::vector<VkPipelineShaderStageCreateInfo> shaderStageCreateInfo(2);
    shaderStageCreateInfo[0].sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
    shaderStageCreateInfo[0].stage = VK_SHADER_STAGE_VERTEX_BIT;
    shaderStageCreateInfo[0].module = vertexShaderModule;
    shaderStageCreateInfo[0].pName = "main";

    shaderStageCreateInfo[1].sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
    shaderStageCreateInfo[1].stage = VK_SHADER_STAGE_FRAGMENT_BIT;
    shaderStageCreateInfo[1].module = fragmentShaderModule;
    shaderStageCreateInfo[1].pName = "main";

    VkPipelineLayoutCreateInfo pipelineLayoutCreateInfo = {};
    pipelineLayoutCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
    pipelineLayoutCreateInfo.setLayoutCount = 1;
    pipelineLayoutCreateInfo.pSetLayouts = &descriptorSetLayout;
    VK_CHECK_RESULT(vkCreatePipelineLayout(device, &pipelineLayoutCreateInfo, NULL, &pipelineLayout));

    VkVertexInputBindingDescription bindingDescription = {};
    bindingDescription.binding = 0;
    bindingDescription.stride = sizeof(Vertex);
    bindingDescription.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;

    std::array<VkVertexInputAttributeDescription, 3> attributeDescriptions = {};
    attributeDescriptions[0].binding = 0;
    attributeDescriptions[0].location = 0;
    attributeDescriptions[0].format = VK_FORMAT_R32G32B32_SFLOAT;    //踩坑，这里要和输入的vertexbuffer一致，不一致的话会扔掉一部分数据
    attributeDescriptions[0].offset = offsetof(Vertex, pos);
    attributeDescriptions[1].binding = 0;
    attributeDescriptions[1].location = 1;
    attributeDescriptions[1].format = VK_FORMAT_R32G32B32_SFLOAT;
    attributeDescriptions[1].offset = offsetof(Vertex, color);
    attributeDescriptions[2].binding = 0;
    attributeDescriptions[2].location = 2;
    attributeDescriptions[2].format = VK_FORMAT_R32G32_SFLOAT;
    attributeDescriptions[2].offset = offsetof(Vertex, texCoord);

    VkPipelineVertexInputStateCreateInfo vertexInputInfo = {};
    vertexInputInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
    vertexInputInfo.vertexBindingDescriptionCount = 1;
    vertexInputInfo.pVertexBindingDescriptions = &bindingDescription; // Optional
    vertexInputInfo.vertexAttributeDescriptionCount = static_cast<uint32_t>(attributeDescriptions.size());
    vertexInputInfo.pVertexAttributeDescriptions = attributeDescriptions.data(); // Optional

    VkPipelineInputAssemblyStateCreateInfo inputAssembly = {};
    inputAssembly.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
    inputAssembly.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
    inputAssembly.primitiveRestartEnable = VK_FALSE;

    VkViewport viewport = {};
    viewport.x = 0.0f;
    viewport.y = 0.0f;
    viewport.width = (float)swapChainExtent.width;
    viewport.height = (float)swapChainExtent.height;
    viewport.minDepth = 0.0f;
    viewport.maxDepth = 1.0f;
    VkRect2D scissor = {};
    scissor.offset = { 0, 0 };
    scissor.extent = swapChainExtent;

    VkPipelineViewportStateCreateInfo viewportState = {};
    viewportState.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
    viewportState.viewportCount = 1;
    viewportState.pViewports = &viewport;
    viewportState.scissorCount = 1;
    viewportState.pScissors = &scissor;

    VkPipelineRasterizationStateCreateInfo rasterizer = {};
    rasterizer.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
    rasterizer.depthClampEnable = VK_FALSE;
    rasterizer.rasterizerDiscardEnable = VK_FALSE;
    rasterizer.polygonMode = VK_POLYGON_MODE_FILL;
    rasterizer.lineWidth = 1.0f;
    rasterizer.cullMode = VK_CULL_MODE_BACK_BIT;
    rasterizer.frontFace = VK_FRONT_FACE_CLOCKWISE;
    rasterizer.depthBiasEnable = VK_FALSE;
    rasterizer.depthBiasConstantFactor = 0.0f; // Optional
    rasterizer.depthBiasClamp = 0.0f; // Optional
    rasterizer.depthBiasSlopeFactor = 0.0f; // Optional

    VkPipelineMultisampleStateCreateInfo multisampling = {};
    multisampling.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
    multisampling.sampleShadingEnable = VK_FALSE;
    multisampling.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;
    multisampling.minSampleShading = 1.0f; // Optional
    multisampling.pSampleMask = nullptr; // Optional
    multisampling.alphaToCoverageEnable = VK_FALSE; // Optional
    multisampling.alphaToOneEnable = VK_FALSE; // Optional

    VkPipelineColorBlendAttachmentState colorBlendAttachment = {};
    colorBlendAttachment.colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;
    colorBlendAttachment.blendEnable = VK_FALSE;
    colorBlendAttachment.srcColorBlendFactor = VK_BLEND_FACTOR_ONE; // Optional
    colorBlendAttachment.dstColorBlendFactor = VK_BLEND_FACTOR_ZERO; // Optional
    colorBlendAttachment.colorBlendOp = VK_BLEND_OP_ADD; // Optional
    colorBlendAttachment.srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE; // Optional
    colorBlendAttachment.dstAlphaBlendFactor = VK_BLEND_FACTOR_ZERO; // Optional
    colorBlendAttachment.alphaBlendOp = VK_BLEND_OP_ADD; // Optional

    VkPipelineDepthStencilStateCreateInfo depthStencilCI = {};
    depthStencilCI.sType = VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO;
    depthStencilCI.depthTestEnable = VK_TRUE;
    depthStencilCI.depthWriteEnable = VK_TRUE;
    depthStencilCI.depthCompareOp = VK_COMPARE_OP_LESS_OR_EQUAL;
    depthStencilCI.depthBoundsTestEnable = VK_FALSE;
    depthStencilCI.minDepthBounds = 0;
    depthStencilCI.maxDepthBounds = 0;
    depthStencilCI.stencilTestEnable = VK_FALSE;

    VkPipelineColorBlendStateCreateInfo colorBlending = {};
    colorBlending.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
    colorBlending.logicOpEnable = VK_FALSE;
    colorBlending.logicOp = VK_LOGIC_OP_COPY; // Optional
    colorBlending.attachmentCount = 1;
    colorBlending.pAttachments = &colorBlendAttachment;
    colorBlending.blendConstants[0] = 0.0f; // Optional
    colorBlending.blendConstants[1] = 0.0f; // Optional
    colorBlending.blendConstants[2] = 0.0f; // Optional
    colorBlending.blendConstants[3] = 0.0f; // Optional

    VkDynamicState dynamicStates[] = {
    VK_DYNAMIC_STATE_VIEWPORT,
    VK_DYNAMIC_STATE_LINE_WIDTH
    };

    VkPipelineDynamicStateCreateInfo dynamicState = {};
    dynamicState.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
    dynamicState.dynamicStateCount = 2;
    dynamicState.pDynamicStates = dynamicStates;

    VkGraphicsPipelineCreateInfo pipelineCreateInfo = {};
    pipelineCreateInfo.sType = VK_STRUCTURE_TYPE_COMPUTE_PIPELINE_CREATE_INFO;
    pipelineCreateInfo.pStages = shaderStageCreateInfo.data();
    pipelineCreateInfo.stageCount = shaderStageCreateInfo.size();
    pipelineCreateInfo.layout = pipelineLayout;
    pipelineCreateInfo.renderPass = renderPass;
    pipelineCreateInfo.pVertexInputState = &vertexInputInfo;
    pipelineCreateInfo.pInputAssemblyState = &inputAssembly;
    pipelineCreateInfo.pViewportState = &viewportState;
    pipelineCreateInfo.pRasterizationState = &rasterizer;
    pipelineCreateInfo.pMultisampleState = &multisampling;
    //pipelineCreateInfo.pDepthStencilState = &depthStencilCI;
    pipelineCreateInfo.pColorBlendState = &colorBlending;
    //pipelineCreateInfo.pDynamicState = &dynamicState;

    pipelineCreateInfo.subpass = 0;
    pipelineCreateInfo.basePipelineHandle = VK_NULL_HANDLE; // Optional
    pipelineCreateInfo.basePipelineIndex = -1; // Optional
    VK_CHECK_RESULT(vkCreateGraphicsPipelines(device, VK_NULL_HANDLE, 1, &pipelineCreateInfo, NULL, &pipeline));
}

void RenderApplication::CreateFrameBuffers()
{
    swapChainFramebuffers.resize(swapChainImageViews.size());
    for (size_t i = 0; i < swapChainImageViews.size(); i++) {

        VkFramebufferCreateInfo framebufferInfo = {};
        framebufferInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
        framebufferInfo.renderPass = renderPass;
        framebufferInfo.attachmentCount = 1;
        framebufferInfo.pAttachments = &swapChainImageViews[i];
        framebufferInfo.width = swapChainExtent.width;
        framebufferInfo.height = swapChainExtent.height;
        framebufferInfo.layers = 1;

        if (vkCreateFramebuffer(device, &framebufferInfo, nullptr, &swapChainFramebuffers[i]) != VK_SUCCESS) {
            throw std::runtime_error("failed to create framebuffer!");
        }
    }
}

void RenderApplication::CreateDrawCommandBuffer()
{
    commandBuffers.resize(swapChainFramebuffers.size());
    VkCommandBufferAllocateInfo allocInfo = {};
    allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
    allocInfo.commandPool = commandPool;
    allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
    allocInfo.commandBufferCount = (uint32_t)commandBuffers.size();
    VK_CHECK_RESULT(vkAllocateCommandBuffers(device, &allocInfo, commandBuffers.data()));

    for (size_t i = 0; i < commandBuffers.size(); i++) {
        VkCommandBufferBeginInfo beginInfo = {};
        beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
        beginInfo.flags = VK_COMMAND_BUFFER_USAGE_SIMULTANEOUS_USE_BIT;
        beginInfo.pInheritanceInfo = nullptr; // Optional
        vkBeginCommandBuffer(commandBuffers[i], &beginInfo);

        VkRenderPassBeginInfo renderPassInfo = {};
        renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
        renderPassInfo.renderPass = renderPass;
        renderPassInfo.framebuffer = swapChainFramebuffers[i];
        renderPassInfo.renderArea.offset = { 0, 0 };
        renderPassInfo.renderArea.extent = swapChainExtent;

        VkClearValue clearColor = { 1.0f, 1.0f, 1.0f, 1.0f };
        renderPassInfo.clearValueCount = 1;
        renderPassInfo.pClearValues = &clearColor;
        vkCmdBeginRenderPass(commandBuffers[i], &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);
        vkCmdBindPipeline(commandBuffers[i], VK_PIPELINE_BIND_POINT_GRAPHICS, pipeline);

        VkBuffer vertexBuffers[] = { vertexBuffer };
        VkDeviceSize offsets[] = { 0 };
        
        vkCmdBindVertexBuffers(commandBuffers[i], 0, 1, vertexBuffers, offsets);
        vkCmdBindIndexBuffer(commandBuffers[i], indexBuffer, 0, VK_INDEX_TYPE_UINT16);
        vkCmdBindDescriptorSets(commandBuffers[i], VK_PIPELINE_BIND_POINT_GRAPHICS, pipelineLayout, 0, 1, &descriptorSet, 0, nullptr);
        vkCmdDrawIndexed(commandBuffers[i], static_cast<uint32_t>(indices.size()), 1, 0, 0, 0);
        //vkCmdDraw(commandBuffers[i], static_cast<uint32_t>(vertices.size()), 1, 0, 0);
        vkCmdEndRenderPass(commandBuffers[i]);
        VK_CHECK_RESULT(vkEndCommandBuffer(commandBuffers[i]));
    }
}

void RenderApplication::CreateFrameSemaphores()
{
    VkSemaphoreCreateInfo semaphoreCI = {};
    semaphoreCI.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;
    VK_CHECK_RESULT(vkCreateSemaphore(device, &semaphoreCI, nullptr, &imageAvailableSemaphore));
    VK_CHECK_RESULT(vkCreateSemaphore(device, &semaphoreCI, nullptr, &renderFinishedSemaphore));
}

VkCommandBuffer RenderApplication::CreateSingleTimeCommandBuffer()
{
    //这个command buffer只是一个临时的命令缓冲区，和绘制的命令是两回事
    VkCommandBufferAllocateInfo allocInfo = {};
    allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
    allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
    allocInfo.commandPool = commandPool;
    allocInfo.commandBufferCount = 1;

    VkCommandBuffer commandBuffer;
    VK_CHECK_RESULT(vkAllocateCommandBuffers(device, &allocInfo, &commandBuffer));

    VkCommandBufferBeginInfo beginInfo = {};
    beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
    beginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;

    VK_CHECK_RESULT(vkBeginCommandBuffer(commandBuffer, &beginInfo));

    return commandBuffer;
}

void RenderApplication::SubmitSingleTimeCommandBuffer(VkCommandBuffer commandBuffer)
{
    VK_CHECK_RESULT(vkEndCommandBuffer(commandBuffer));
    VkSubmitInfo submitInfo = {};
    submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
    submitInfo.commandBufferCount = 1;
    submitInfo.pCommandBuffers = &commandBuffer;

    VK_CHECK_RESULT(vkQueueSubmit(queue, 1, &submitInfo, VK_NULL_HANDLE));
    VK_CHECK_RESULT(vkQueueWaitIdle(queue));
    vkFreeCommandBuffers(device, commandPool, 1, &commandBuffer);
}

void RenderApplication::CopyBufferToImage(VkBuffer buffer, VkImage image, uint32_t width, uint32_t height)
{
    VkCommandBuffer commandBuffer = CreateSingleTimeCommandBuffer();
    VkBufferImageCopy region = {};
    region.bufferOffset = 0;
    region.bufferRowLength = 0;
    region.bufferImageHeight = 0;

    region.imageSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
    region.imageSubresource.mipLevel = 0;
    region.imageSubresource.baseArrayLayer = 0;
    region.imageSubresource.layerCount = 1;

    region.imageOffset = { 0, 0, 0 };
    region.imageExtent = {
        width,
        height,
        1
    };
    vkCmdCopyBufferToImage(
        commandBuffer,
        buffer,
        image,
        VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
        1,
        &region
    );
    SubmitSingleTimeCommandBuffer(commandBuffer);
}

void RenderApplication::TransitionImageLayout(VkImage image, VkFormat format, VkImageLayout oldLayout, VkImageLayout newLayout)
{
    VkCommandBuffer commandBuffer = CreateSingleTimeCommandBuffer();
    VkImageMemoryBarrier barrier = {};
    barrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
    barrier.oldLayout = oldLayout;
    barrier.newLayout = newLayout;
    barrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;//如果不是特定要求，一定要设置，这里不是默认值
    barrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
    barrier.image = image;
    barrier.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
    barrier.subresourceRange.baseMipLevel = 0;
    barrier.subresourceRange.levelCount = 1;
    barrier.subresourceRange.baseArrayLayer = 0;
    barrier.subresourceRange.layerCount = 1;
    barrier.srcAccessMask = 0; // TODO
    barrier.dstAccessMask = 0; // TODO

    VkPipelineStageFlags sourceStage;
    VkPipelineStageFlags destinationStage;

    if (oldLayout == VK_IMAGE_LAYOUT_UNDEFINED && newLayout == VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL) {
        barrier.srcAccessMask = 0;
        barrier.dstAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;

        sourceStage = VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT;
        destinationStage = VK_PIPELINE_STAGE_TRANSFER_BIT;
    }
    else if (oldLayout == VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL && newLayout == VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL) {
        barrier.srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
        barrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;

        sourceStage = VK_PIPELINE_STAGE_TRANSFER_BIT;
        destinationStage = VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT;
    }
    else {
        throw std::invalid_argument("unsupported layout transition!");
    }

    vkCmdPipelineBarrier(
        commandBuffer,
        sourceStage, destinationStage,
        0,
        0, nullptr,
        0, nullptr,
        1, &barrier
    );

    SubmitSingleTimeCommandBuffer(commandBuffer);

}

VkImageView RenderApplication::CreateImageView(VkImage image, VkFormat format)
{
    VkImageView imageView;

    VkImageViewCreateInfo colorImageViewCI = {};
    colorImageViewCI.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
    colorImageViewCI.image = image;
    colorImageViewCI.viewType = VK_IMAGE_VIEW_TYPE_2D;
    colorImageViewCI.format = format;
    colorImageViewCI.components.r = VK_COMPONENT_SWIZZLE_R;
    colorImageViewCI.components.g = VK_COMPONENT_SWIZZLE_G;
    colorImageViewCI.components.b = VK_COMPONENT_SWIZZLE_B;
    colorImageViewCI.components.a = VK_COMPONENT_SWIZZLE_A;
    colorImageViewCI.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
    colorImageViewCI.subresourceRange.baseMipLevel = 0;
    colorImageViewCI.subresourceRange.levelCount = 1;
    colorImageViewCI.subresourceRange.baseArrayLayer = 0;
    colorImageViewCI.subresourceRange.layerCount = 1;
    VK_CHECK_RESULT(vkCreateImageView(device, &colorImageViewCI, nullptr, &imageView));

    return imageView;
}

void RenderApplication::CreateImage(uint32_t width, uint32_t height, VkFormat format, VkImageTiling tiling, VkImageUsageFlags usage, VkMemoryPropertyFlags properties, VkImage& image, VkDeviceMemory& imageMemory)
{
    VkImageCreateInfo imageInfo = {};
    imageInfo.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
    imageInfo.imageType = VK_IMAGE_TYPE_2D;
    imageInfo.extent.width = width;
    imageInfo.extent.height = height;
    imageInfo.extent.depth = 1;
    imageInfo.mipLevels = 1;
    imageInfo.arrayLayers = 1;
    imageInfo.format = format;
    imageInfo.tiling = tiling;
    imageInfo.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
    imageInfo.usage = usage;
    imageInfo.samples = VK_SAMPLE_COUNT_1_BIT;
    imageInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

    VK_CHECK_RESULT(vkCreateImage(device, &imageInfo, nullptr, &image));

    VkMemoryRequirements memRequirements;
    vkGetImageMemoryRequirements(device, image, &memRequirements);

    VkMemoryAllocateInfo allocInfo = {};
    allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
    allocInfo.allocationSize = memRequirements.size;
    allocInfo.memoryTypeIndex = findMemoryType(memRequirements.memoryTypeBits, properties);

    if (vkAllocateMemory(device, &allocInfo, nullptr, &imageMemory) != VK_SUCCESS) {
        throw std::runtime_error("failed to allocate image memory!");
    }

    vkBindImageMemory(device, image, imageMemory, 0);
}

void RenderApplication::CreateTextureImage()
{
    int texWidth, texHeight, texChannels;
    stbi_uc* pixels = stbi_load(TEXTURE_PATH.c_str(), &texWidth, &texHeight, &texChannels, STBI_rgb);
    VkDeviceSize imageSize = texWidth * texHeight * texChannels;

    if (!pixels) {
        throw std::runtime_error("failed to load texture image!");
    }
    VkBuffer stagingBuffer;
    VkDeviceMemory stagingBufferMemory;
    CreateBuffer(imageSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, stagingBuffer, stagingBufferMemory);

    void* data;
    vkMapMemory(device, stagingBufferMemory, 0, imageSize, 0, &data);
    memcpy(data, pixels, static_cast<size_t>(imageSize));
    vkUnmapMemory(device, stagingBufferMemory);
    stbi_image_free(pixels);

    CreateImage(texWidth, texHeight, VK_FORMAT_R32G32B32_UINT, VK_IMAGE_TILING_OPTIMAL, VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, textureImage, textureImageMemory);
    TransitionImageLayout(textureImage, VK_FORMAT_R32G32B32_UINT, VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL);
    CopyBufferToImage(stagingBuffer, textureImage, static_cast<uint32_t>(texWidth), static_cast<uint32_t>(texHeight));
    TransitionImageLayout(textureImage, VK_FORMAT_R32G32B32_UINT, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);

    vkDestroyBuffer(device, stagingBuffer, nullptr);
    vkFreeMemory(device, stagingBufferMemory, nullptr);
}

void RenderApplication::CreateTextureImageView()
{
    textureImageView = CreateImageView(textureImage, VK_FORMAT_R32G32B32_UINT);
}

void RenderApplication::CreateTextureSampler()
{
    VkSamplerCreateInfo samplerInfo = {};
    samplerInfo.sType = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO;
    samplerInfo.magFilter = VK_FILTER_LINEAR;
    samplerInfo.minFilter = VK_FILTER_LINEAR;
    samplerInfo.addressModeU = VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE;
    samplerInfo.addressModeV = VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE;
    samplerInfo.addressModeW = VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE;
    samplerInfo.anisotropyEnable = VK_FALSE;
    samplerInfo.maxAnisotropy = 1;
    samplerInfo.borderColor = VK_BORDER_COLOR_INT_OPAQUE_WHITE;
    samplerInfo.unnormalizedCoordinates = VK_FALSE;
    samplerInfo.compareEnable = VK_FALSE;
    samplerInfo.compareOp = VK_COMPARE_OP_ALWAYS;
    samplerInfo.mipmapMode = VK_SAMPLER_MIPMAP_MODE_LINEAR;
    samplerInfo.mipLodBias = 0.0f;
    samplerInfo.minLod = 0.0f;
    samplerInfo.maxLod = 0.0f;
    VK_CHECK_RESULT(vkCreateSampler(device, &samplerInfo, nullptr, &textureSampler));
}

void RenderApplication::UpdateUniformBuffer()
{
    UniformBufferObject ubo;
    static auto startTime = std::chrono::high_resolution_clock::now();

    auto currentTime = std::chrono::high_resolution_clock::now();
    float time = std::chrono::duration_cast<std::chrono::milliseconds>(currentTime - startTime).count() / 1000.0f;
    ubo.model = glm::rotate(glm::mat4(1.0f), time * glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
    ubo.view = glm::lookAt(glm::vec3(2.0f, 2.0f, 2.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f));
    ubo.proj = glm::perspective(glm::radians(45.0f), swapChainExtent.width / (float)swapChainExtent.height, 0.1f, 10.0f);

    void* data;
    vkMapMemory(device, uniformBufferMemory, 0, sizeof(ubo), 0, &data);
    memcpy(data, &ubo, sizeof(ubo));
    vkUnmapMemory(device, uniformBufferMemory);
}

void RenderApplication::DrawFrame()
{
    uint32_t imageIndex;
    VK_CHECK_RESULT(vkAcquireNextImageKHR(device, swapChain, 1000000000, imageAvailableSemaphore, VK_NULL_HANDLE, &imageIndex));
    VkSubmitInfo submitInfo = {};
    submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;

    VkSemaphore waitSemaphores[] = { imageAvailableSemaphore };
    VkPipelineStageFlags waitStages[] = { VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT };
    submitInfo.waitSemaphoreCount = 1;
    submitInfo.pWaitSemaphores = waitSemaphores;
    submitInfo.pWaitDstStageMask = waitStages;
    submitInfo.commandBufferCount = 1;
    submitInfo.pCommandBuffers = &commandBuffers[imageIndex];

    VkSemaphore signalSemaphores[] = { renderFinishedSemaphore };
    submitInfo.signalSemaphoreCount = 1;
    submitInfo.pSignalSemaphores = signalSemaphores;
    VK_CHECK_RESULT(vkQueueSubmit(queue, 1, &submitInfo, VK_NULL_HANDLE));

    VkPresentInfoKHR presentInfo = {};
    presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
    presentInfo.waitSemaphoreCount = 1;
    presentInfo.pWaitSemaphores = signalSemaphores;

    VkSwapchainKHR swapChains[] = { swapChain };
    presentInfo.swapchainCount = 1;
    presentInfo.pSwapchains = swapChains;
    presentInfo.pImageIndices = &imageIndex;
    presentInfo.pResults = nullptr; // Optional
    VK_CHECK_RESULT(vkQueuePresentKHR(queue, &presentInfo));
}
