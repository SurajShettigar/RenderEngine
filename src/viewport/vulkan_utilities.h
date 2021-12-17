#define GLFW_INCLUDE_VULKAN
#include <vulkan/vulkan.h>
#include <GLFW/glfw3.h>
#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEFAULT_ALIGNED_GENTYPES
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <array>
#include <vector>
#include <set>
#include <limits>
#include <algorithm>
#include <string.h>
#include <chrono>

#include <iostream>

#include "../utils/file_utils.h"

using std::array;
using std::cerr;
using std::cout;
using std::endl;
using std::vector;

static const std::string VERTEX_SHADER_PATH = "./shaders/shader.vert.spv";
static const std::string FRAGMENT_SHADER_PATH = "./shaders/shader.frag.spv";

const vector<const char *> DEVICE_EXTENSIONS{
    VK_KHR_SWAPCHAIN_EXTENSION_NAME};

const vector<const char *> VALIDATION_LAYERS = {
    "VK_LAYER_KHRONOS_validation"};

struct VulkanDevice
{
    VkPhysicalDevice physicalDevice;
    VkDevice logicalDevice;
};

struct QueueFamilyIndices
{
    int graphicsFamily = -1;
    int presentationFamily = -1;

    bool isValid()
    {
        return graphicsFamily >= 0 && presentationFamily >= 0;
    }
};

struct SwapchainDetails
{
    VkSurfaceCapabilitiesKHR surfaceCapabilities;
    vector<VkSurfaceFormatKHR> fromats;
    vector<VkPresentModeKHR> presentationModes;
};

struct SwapchainImage
{
    VkImage image;
    VkImageView imageView;
};

struct VertexData
{
    glm::vec2 position;
    glm::vec3 color;

    static VkVertexInputBindingDescription getBindingDescription()
    {
        VkVertexInputBindingDescription desc{};

        desc.binding = 0;
        desc.stride = sizeof(VertexData);
        desc.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;

        return desc;
    }

    static array<VkVertexInputAttributeDescription, 2> getAttributeDescription()
    {
        array<VkVertexInputAttributeDescription, 2> desc{};

        desc[0].binding = 0;
        desc[0].format = VK_FORMAT_R32G32_SFLOAT;
        desc[0].location = 0;
        desc[0].offset = offsetof(VertexData, position);

        desc[1].binding = 0;
        desc[1].format = VK_FORMAT_R32G32B32_SFLOAT;
        desc[1].location = 1;
        desc[1].offset = offsetof(VertexData, color);

        return desc;
    }
};

struct TransformMatrices
{
    alignas(16) glm::mat4 model;
    alignas(16) glm::mat4 view;
    alignas(16) glm::mat4 projection;
};

static VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(
    VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
    VkDebugUtilsMessageTypeFlagsEXT messageType,
    const VkDebugUtilsMessengerCallbackDataEXT *pCallbackData,
    void *pUserData)
{
    std::cerr << "validation layer: " << pCallbackData->pMessage << std::endl;
    return VK_FALSE;
}

static uint32_t findMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties, const VkPhysicalDevice &physicalDevice)
{
    VkPhysicalDeviceMemoryProperties memProps;
    vkGetPhysicalDeviceMemoryProperties(physicalDevice, &memProps);

    for (uint32_t i = 0; i < memProps.memoryTypeCount; i++)
    {
        if ((typeFilter & (1 << i)) && ((memProps.memoryTypes[i].propertyFlags & properties) == properties))
        {
            return i;
        }
    }

    throw std::runtime_error("Failed to find suitable memory type.");
}

static void createBuffer(VkDeviceSize size,
                         VkBufferUsageFlags usage,
                         VkMemoryPropertyFlags memoryProperties,
                         VulkanDevice device,
                         VkBuffer &buffer,
                         VkDeviceMemory &bufferMemory)
{
    VkBufferCreateInfo bufferCreateInfo{};
    bufferCreateInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
    bufferCreateInfo.pNext = nullptr;
    bufferCreateInfo.flags = 0;
    bufferCreateInfo.size = size;
    bufferCreateInfo.usage = usage;
    bufferCreateInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

    if (vkCreateBuffer(device.logicalDevice, &bufferCreateInfo, nullptr, &buffer) != VK_SUCCESS)
        throw RendererException("Failed to create vertex buffer");

    VkMemoryRequirements memRequirements;
    vkGetBufferMemoryRequirements(device.logicalDevice, buffer, &memRequirements);

    VkMemoryAllocateInfo memAllocateInfo{};
    memAllocateInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
    memAllocateInfo.pNext = nullptr;
    memAllocateInfo.allocationSize = memRequirements.size;
    memAllocateInfo.memoryTypeIndex = findMemoryType(memRequirements.memoryTypeBits,
                                                     memoryProperties,
                                                     device.physicalDevice);

    if (vkAllocateMemory(device.logicalDevice, &memAllocateInfo, nullptr, &bufferMemory) != VK_SUCCESS)
        throw RendererException("Failed to allocate memory for vertex buffer.");

    vkBindBufferMemory(device.logicalDevice, buffer, bufferMemory, 0);
}