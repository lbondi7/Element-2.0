//
// Created by lbondi7 on 09/11/2020.
//

#include "VknResources.h"

Element::DescriptorSet *Element::VknResources::allocateDescriptorSet() {

    auto& set = descriptorSets.emplace_back(std::make_unique<DescriptorSet>());

    return set.get();
}

Element::VknDescriptorPool *Element::VknResources::allocateDescriptorPool(
        const PipelineData& pipelineData, uint32_t imageCount) {

    for (auto& pool : descriptorPools)
    {
        if(pipelineData == pool->pipelineData) {
            if(pool->isFlushed())
                pool->init(pipelineData, imageCount);

            return pool.get();
        }
    }

    auto& pool = descriptorPools.emplace_back();
    pool = std::make_unique<VknDescriptorPool>();
    pool->init(pipelineData, imageCount);
    return pool.get();
}

void Element::VknResources::flush() {

    for (auto& pool : descriptorPools)
    {
        pool->flush();
    }

}

void Element::VknResources::destroy() {
    for (auto& pool : descriptorPools)
    {
        pool->destroy();
    }
}
