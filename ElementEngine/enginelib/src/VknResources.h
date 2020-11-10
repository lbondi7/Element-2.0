//
// Created by lbondi7 on 09/11/2020.
//

#pragma once

#include "VknDescriptorPool.h"
#include "DescriptorSet.h"

#include <memory>

namespace Element {
    class VknResources {

    public:
        VknResources() = default;
        ~VknResources() = default;

//        static VknResources& get(){
//            static VknResources resources;
//            return resources;
//        }

        DescriptorSet* allocateDescriptorSet();

        VknDescriptorPool* allocateDescriptorPool(const PipelineData& pipelineData,
                                                 uint32_t imageCount);

        void flush();

        void destroy();

    private:

        std::vector<std::unique_ptr<DescriptorSet>> descriptorSets;
        std::vector<std::unique_ptr<VknDescriptorPool>> descriptorPools;
    };
}

//ELEMENTENGINE_VKNRESOURCES_H
