#pragma once

#include <vulkan/vulkan.h>

#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>

#include "VkInitializers.h"

#include <vector>
#include <array>
#include <optional>

#include <element/Maths/Vec3.h>

namespace Element {
    struct QueueIndices {

        enum QueueType {
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
        glm::vec3 normal;
        glm::vec2 texCoord;

        bool operator==(const Vertex &other) const = default;

        Vertex &operator=(const Vertex &other) = default;

        static VkVertexInputBindingDescription getBindingDescription() {
            VkVertexInputBindingDescription bindingDescription = VkInitializers::vertexInputBindingDescription(
                    VK_VERTEX_INPUT_RATE_VERTEX, 0, sizeof(Vertex));
            return bindingDescription;
        }

        static std::array<VkVertexInputAttributeDescription, 3> getAttributeDescriptions() {
            std::array<VkVertexInputAttributeDescription, 3> attributeDescriptions{};

            attributeDescriptions[0] = VkInitializers::vertexInputAttributeDescription(
                    0, 0, VK_FORMAT_R32G32B32_SFLOAT, offsetof(Vertex, pos));
            attributeDescriptions[1] = VkInitializers::vertexInputAttributeDescription(
                    0, 1, VK_FORMAT_R32G32B32_SFLOAT, offsetof(Vertex, normal));
            attributeDescriptions[2] = VkInitializers::vertexInputAttributeDescription(
                    0, 2, VK_FORMAT_R32G32_SFLOAT, offsetof(Vertex, texCoord));

            return attributeDescriptions;
        }
    };

    struct UniformBufferObject {
        alignas(16) glm::mat4 model = glm::mat4(1.0f);
    };

    enum class DirtyFlags : int {
        CLEAN = 0,
        DIRTY = 1,
        PIPELINE_DIRTY = 2,
    };
}