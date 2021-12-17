#ifndef VULKAN_RENDERER_H
#define VULKAN_RENDERER_H

#define GLFW_INCLUDE_VULKAN
#include "renderer.h"
#include "vulkan_utilities.h"

class VulkanRenderer : Renderer
{
private:
    static const int MAX_FRAMES_IN_FLIGHT = 2;

    const vector<VertexData> vertices = {
        {{-0.5f, -0.5f}, {1.0f, 0.0f, 0.0f}},
        {{0.5f, -0.5f}, {0.0f, 1.0f, 0.0f}},
        {{0.5f, 0.5f}, {0.0f, 0.0f, 1.0f}},
        {{-0.5f, 0.5f}, {1.0f, 1.0f, 1.0f}}};
    const vector<uint16_t> indices = {0, 1, 2, 2, 3, 0};

    // vector<VertexData> vertices;
    // vector<uint32_t> indices;


    GLFWwindow *window;

    VkInstance instance;
    VulkanDevice mainDevice;

    VkSurfaceKHR surface;

    VkQueue graphicsQueue;
    VkQueue presentationQueue;

    VkSwapchainKHR swapchain;
    VkFormat swapchainFormat;
    VkExtent2D swapchainExtent;
    vector<SwapchainImage> swapchainImages;

    VkRenderPass renderPass;
    VkPipelineLayout pipelineLayout;
    VkPipeline graphicsPipeline;
    vector<VkFramebuffer> swapchainFramebuffers;

    VkDescriptorSetLayout descriptorSetLayout;
    VkDescriptorPool descriptorPool;
    vector<VkDescriptorSet> descriptorSets;

    VkCommandPool commandPool;
    vector<VkCommandBuffer> commandBuffers;

    VkBuffer vertexBuffer;
    VkDeviceMemory vertexBufferMemory;
    VkBuffer indexBuffer;
    VkDeviceMemory indexBufferMemory;
    vector<VkBuffer> uniformBuffers;
    vector<VkDeviceMemory> uniformBuffersMemory;

    vector<VkSemaphore> imageAvailableSemaphores;
    vector<VkSemaphore> renderFinishedSemaphores;
    vector<VkFence> inFlightFences;
    vector<VkFence> imagesInFlight;
    size_t currentFrame = 0;

#ifdef NDEBUG
    static const bool isValidationLayersEnabled = false;
#else
    static const bool isValidationLayersEnabled = true;
#endif
    bool checkValidationLayersSupport();

    VkDebugUtilsMessengerEXT debugMessenger;
    void populateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT &createInfo);
    void createDebugMessenger() throw();
    void destoryDebugMessenger() throw();

    vector<const char *> getRequiredExtensions();
    bool hasRequiredExtenstions(const vector<const char *> &requiredExtensions);
    void createVulkanInstance() throw();

    void createSurface() throw();

    QueueFamilyIndices getQueueFamilies(const VkPhysicalDevice &device);
    bool isDeviceExtensionSupported(const VkPhysicalDevice &device);
    SwapchainDetails getSwapchainDetails(const VkPhysicalDevice &device);
    bool isDeviceSuitable(const VkPhysicalDevice &device);
    void getPhysicalDevice() throw();

    void createLogicalDevice() throw();

    VkSurfaceFormatKHR chooseBestSurfaceFormat(const vector<VkSurfaceFormatKHR> &formats);
    VkPresentModeKHR chooseBestPresentationMode(const vector<VkPresentModeKHR> &presentationModes);
    VkExtent2D chooseSwapExtent(const VkSurfaceCapabilitiesKHR &surfaceCapabilites);
    VkImageView createImageView(VkImage image, VkFormat format, VkImageAspectFlags aspectFlags) throw();
    void createSwapchain() throw();

    VkShaderModule createShaderModule(const vector<char> &code) throw();
    void createRenderPass();
    void createDescriptorSetLayout();
    void createGraphicsPipeline();
    void createFramebuffers();

    void createCommandPool();
    void copyBuffer(VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size);
    void createVertexBuffer();
    void createIndexBuffer();
    void createUniformBuffers();
    void createDescriptorPools();
    void createDescriptorSets();
    void createCommandBuffers();

    void createSyncObjects();
    void updateUniformBuffers(uint32_t imageIndex);
    void drawFrame();

    void cleanSwapchain();
    void recreateSwapchain();

protected:
public:
    bool frameBufferResized;

    VulkanRenderer() = default;
    void start(GLFWwindow *window) override;
    void update() override;
    void render() override;
    void destroy() override;

    void onWindowResized() override;
    void onModelLoaded(const std::unordered_map<math::Point, uint32_t> &uniqueVertices) override;
    void onInput(InputType input, float value_x, float value_y) override;
};

#endif