//
// Created by lbondi7 on 11/11/2020.
//

#pragma once

#include <Vulkan/VknBuffer.h>
#include <Vulkan/Pipeline/VknDescriptorSet.h>

#include <element/Light.h>

#include <vector>
#include <memory>
#include <queue>
#include <map>


namespace  Element {
    class VknPipeline;
}

namespace  Element {
    struct LightData{
        alignas(16) Vec3 col;
        float padding1 = 0.0f;
        alignas(16) Vec3 pos;
        float padding2 = 0.0f;
        alignas(16) Vec3 attenuation;
        float padding3 = 0.0f;
        alignas(16) Vec3 direction;
        alignas(4) float cutOff;
        alignas(4) float outerCutOff;
        alignas(4) float intensity;
        alignas(4) int type;
        int padding4 = 0;
    };

    struct LightConstants{
        //alignas(16) Vec3 camPos = Vec3(0.0f);
        alignas(4) int number;
    };

    class LightManager {

    public:
        explicit LightManager(VknPipeline* pipeline, uint32_t imageCount);
        ~LightManager() = default;

        void init(VknPipeline* pipeline, uint32_t imageCount);

        void reInit(uint32_t imageCount);

        void update(uint32_t imageIndex);

        Light* getLight();

        std::vector<std::unique_ptr<Light>>& getLights();

        std::vector<VknBuffer>& getLightBuffers();
        std::vector<VknBuffer>& getLightConstantsBuffers();

        void deInit();

        void addDescriptorSet(VknPipeline* pipeline, uint32_t imageCount);

        VknDescriptorSet* getDescriptorSet(const std::string& pipelineName);

    private:

        std::map<const std::string, std::unique_ptr<VknDescriptorSet>> descriptorSets;

        uint32_t lightCount = 0;
        std::vector<std::unique_ptr<Light>> lights;
        std::vector<VknBuffer> lightBuffers;
        std::vector<VknBuffer> lightConstantsBuffers;
        std::queue<int> free_position;
    };
}

//ELEMENTENGINE_LIGHTMANAGER_H
