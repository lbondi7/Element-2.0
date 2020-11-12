//
// Created by lbondi7 on 08/11/2020.
//

#pragma once

#include <element/Camera.h>

#include "Buffer.h"
#include "DescriptorSet.h"
#include "VknPipeline.h"

#include <glm/glm.hpp>

namespace Element {
    class EleCamera : public Camera {
    public:
        EleCamera();
        explicit EleCamera(CameraType _type);
        explicit EleCamera(CameraType _type, const glm::vec4& viewport, const glm::vec4& rect);
        explicit EleCamera(CameraType _type, VknPipeline* pipeline);
        void update(float windowWidth, float windowHeight, uint32_t imageIndex);
        [[nodiscard]] const glm::vec4& getViewport() const;
        [[nodiscard]] const glm::vec4& getRect() const;
        [[nodiscard]] const glm::mat4& getViewMatrix() const;
        [[nodiscard]] const glm::mat4& getProjMatrix() const;
        void setRect(glm::vec4 _rect);
        void setViewport(glm::vec4 _viewport);

//        std::vector<Buffer> uniformBuffers;

        const VkViewport &getVkViewport() const;

        void setVkViewport(const VkViewport &vkViewport);

        void setVkViewport(float x,float y,float w,float h, float minDepth = 0.0f, float maxDepth = 1.0f);

        const VkRect2D &getScissorRect() const;

        void setScissorRect(const VkRect2D &scissorRect);

        void initDescSet(VknPipeline* pipeline, uint32_t imageCount, int id);

        std::vector<Buffer> uniformBuffers;

        DescriptorSet* descriptorSet;
    private:

        VkViewport vkViewport;
        VkRect2D scissorRect;

        glm::vec4 viewport;
        glm::vec4 rect;
        glm::mat4 transformMatrix  = glm::mat4(1.0f);
        glm::mat4 viewMatrix  = glm::mat4(1.0f);
        glm::mat4 projectionMatrix  = glm::mat4(1.0f);
        glm::mat4 inverted;


    };
}

 //ELEMENTENGINE_ELECAMERA_H
