#pragma once

#include <vulkan/vulkan.h>

#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>

#include "VkInitializers.h"

#include <vector>
#include <array>
#include <optional>

const std::vector<const char*> validationLayers = {
    "VK_LAYER_KHRONOS_validation",
    "VK_LAYER_LUNARG_monitor"
};

const std::vector<const char*> deviceExtensions = {
    VK_KHR_SWAPCHAIN_EXTENSION_NAME,
};

#ifdef NDEBUG
const bool enableValidationLayers = true;
#else
const bool enableValidationLayers = true;
#endif

struct QueueIndices {

    enum QueueType
    {
        GRAPHICS = 0,
        COMPUTE = 1,
        TRANSFER = 2,
        GRAPHICS_COMPUTE = 3,
        ALL = 4,
    };

    std::optional<uint32_t> graphics;
    std::optional<uint32_t> present;
    std::optional<uint32_t> compute;
    std::optional<uint32_t> transfer;

    bool canRender() {
        return graphics.has_value() && present.has_value();
    }

    bool canCompute() {
        return compute.has_value();
    }

    bool canRenderandCompute() {
        return graphics.has_value() && present.has_value() && compute.has_value();
    }

    bool canTransfer() {
        return transfer.has_value();
    }

    bool isComplete() {
        return graphics.has_value() && present.has_value() && compute.has_value() && transfer.has_value();
    }
};

struct SwapChainSupportDetails {
    VkSurfaceCapabilitiesKHR capabilities;
    std::vector<VkSurfaceFormatKHR> formats;
    std::vector<VkPresentModeKHR> presentModes;
};


struct Vertex {
    glm::vec3 pos;
    glm::vec3 color;
    glm::vec2 texCoord;

    bool operator==(const Vertex& other) const {
        return pos == other.pos && color == other.color && texCoord == other.texCoord;
    }

    Vertex& operator=(const Vertex& other){
        this->pos = other.pos;
        this->color = other.color;
        this->texCoord = other.texCoord;
        return *this;
    }

    static VkVertexInputBindingDescription getBindingDescription() {
        VkVertexInputBindingDescription bindingDescription = Element::VkInitializers::vertexInputBindingDescription(VK_VERTEX_INPUT_RATE_VERTEX, 0, sizeof(Vertex));
        return bindingDescription;
    }

    static std::array<VkVertexInputAttributeDescription, 3> getAttributeDescriptions() {
        std::array<VkVertexInputAttributeDescription, 3> attributeDescriptions{};
        
        attributeDescriptions[0] = Element::VkInitializers::vertexInputAttributeDescription(0, 0, VK_FORMAT_R32G32B32_SFLOAT, offsetof(Vertex, pos));
        attributeDescriptions[1] = Element::VkInitializers::vertexInputAttributeDescription(0, 1, VK_FORMAT_R32G32B32_SFLOAT, offsetof(Vertex, color));
        attributeDescriptions[2] = Element::VkInitializers::vertexInputAttributeDescription(0, 2, VK_FORMAT_R32G32_SFLOAT, offsetof(Vertex, texCoord));

        return attributeDescriptions;
    }
};

struct UniformBufferObject {
    alignas(16) glm::mat4 model = glm::mat4(1.0f);
    alignas(16) glm::mat4 view = glm::mat4(1.0f);
    alignas(16) glm::mat4 proj = glm::mat4(1.0f);
};


enum class DirtyFlags : int {
    CLEAN = 0,
    DIRTY = 1,
    PIPELINE_DIRTY = 2,
};

