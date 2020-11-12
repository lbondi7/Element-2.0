//
// Created by lbondi7 on 11/11/2020.
//

#pragma once

#include "Buffer.h"
#include "DescriptorSet.h"

#include <element/Light.h>

#include <vector>
#include <memory>
#include <queue>

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
        explicit LightManager(uint32_t imageCount);
        ~LightManager() = default;

        void init(uint32_t imageCount);

        void update(const Vec3& camPos, uint32_t imageIndex);

        Light* getLight();

        std::vector<std::unique_ptr<Light>>& getLights();

        std::vector<Buffer>& getLightBuffers();
        std::vector<Buffer>& getLightConstantsBuffers();

        void deInit();

        DescriptorSet* descriptorSet;

    private:

        uint32_t lightCount = 0;
        std::vector<std::unique_ptr<Light>> lights;
        std::vector<Buffer> lightBuffers;
        std::vector<Buffer> lightConstantsBuffers;
        std::queue<int> free_position;
    };
}

//ELEMENTENGINE_LIGHTMANAGER_H
