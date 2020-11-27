//
// Created by lbondi7 on 08/11/2020.
//

#pragma once

#include <element/Camera.h>

#include <Vulkan/VknBuffer.h>
#include <Vulkan/Pipeline/VknDescriptorSet.h>

#include <element/Maths/Vec4.h>
#include <element/Maths/Vec2.h>
#include <glm/glm.hpp>

namespace Element{
    class VknPipeline;
}

namespace Element {
    class EleCamera : public Camera {
    public:
        explicit EleCamera(CameraType _type, VknPipeline* pipeline, uint32_t imageCount);
        ~EleCamera() override;

        void init(CameraType _type, VknPipeline* pipeline, uint32_t imageCount);
        void update(float windowWidth, float windowHeight, uint32_t imageIndex);
        [[nodiscard]] const glm::mat4& getViewMatrix() const;
        [[nodiscard]] const glm::mat4& getProjMatrix() const;

//        std::vector<Buffer> uniformBuffers;

        const VkViewport &getVkViewport() const;

        void setVkViewport(const VkViewport &vkViewport);

        void setVkViewport(float x,float y,float w,float h, float minDepth = 0.0f, float maxDepth = 1.0f);

        const VkRect2D &getScissorRect() const;

        void setScissorRect(const VkRect2D &scissorRect);

        void initDescSet(VknPipeline* pipeline, uint32_t imageCount, int id);

        VknDescriptorSet* getDescriptorSet(const std::string& pipelineName);

        void reInit(uint32_t imageCount);

        void destroy() override;

        void setViewportandRect(VkCommandBuffer vkCmdBuffer, Vec2 windowSize);

    private:

        std::vector<VknBuffer> uniformBuffers;

        std::unique_ptr<VknDescriptorSet> descriptorSet;
        void addDescriptorSet(VknPipeline* pipeline, uint32_t imageCount);

        std::map<std::string, std::unique_ptr<VknDescriptorSet>> descriptorSets;


        VkViewport vkViewport = {0, 0, 1, 1};
        VkRect2D scissorRect = {0, 0, 1, 1};

        glm::mat4 transformMatrix = glm::mat4(1.0f);
        glm::mat4 viewMatrix = glm::mat4(1.0f);
        glm::mat4 projectionMatrix = glm::mat4(1.0f);
        glm::mat4 inverted = glm::mat4(1.0f);

    };
}

 //ELEMENTENGINE_ELECAMERA_H
