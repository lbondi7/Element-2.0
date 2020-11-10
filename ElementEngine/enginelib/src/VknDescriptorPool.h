//
// Created by lbondi7 on 09/11/2020.
//

#pragma once

#include "PipelineData.h"

#include <vulkan/vulkan.h>
#include <vector>

namespace Element {

    class VknDescriptorPool {

    public:
        VknDescriptorPool() = default;

        ~VknDescriptorPool();

        void init(const PipelineData& pipelineData,
                  uint32_t imageCount);

        bool isFull(uint32_t setCount);

        void flush();

        void destroy();

        VkDescriptorPool getVkDescriptorPool();

        bool isFlushed() const;

    private:

        VkDescriptorPool m_vkDescriptorPool;
        uint32_t maxSets;
        uint32_t currentSetCount;
        uint32_t id;
        bool full;
        bool flushed = true;
    };
}

//ELEMENTENGINE_VKNDESCRIPTORPOOL_H
