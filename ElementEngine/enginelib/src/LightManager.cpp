//
// Created by lbondi7 on 11/11/2020.
//

#include "LightManager.h"

const int MAX_LIGHT_COUNT = 100;

Element::LightManager::LightManager(uint32_t imageCount) {

    init(imageCount);

}

void Element::LightManager::init(uint32_t imageCount) {

    lightBuffers.resize(imageCount);
    lightConstantsBuffers.resize(imageCount);

    for (int i = 0; i < imageCount; ++i) {

        lightBuffers[i].Create(static_cast<VkDeviceSize>(sizeof(LightData) * MAX_LIGHT_COUNT), 0,
                               VK_BUFFER_USAGE_STORAGE_BUFFER_BIT);
        lightBuffers[i].Map();

        lightConstantsBuffers[i].Create(sizeof(LightConstants), 0, VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT);
        lightConstantsBuffers[i].Map();
    }

}


void Element::LightManager::update(uint32_t imageIndex){

    LightData l[MAX_LIGHT_COUNT];
    int i = 0;
    int position = 0;
    for(const auto& light : lights){
        if(!light) {
            free_position.emplace(position);
            ++position;
            continue;
        }

       l[i].pos = light->getPosition();
       l[i].direction = light->getDirection();
       l[i].type = static_cast<int>(light->getType());
       l[i].attenuation = Vec3(light->getConstant(), light->getLinear(), light->getQuadratic());
       l[i].col = light->getColour();
       l[i].intensity = light->getIntensity();
       l[i].cutOff = light->getCutOff();
       l[i].outerCutOff = light->getOuterCutOff();
       ++i;
       ++position;
    }

    lightBuffers[imageIndex].CopyMemory(&l, sizeof(LightData) * i);
    lightConstantsBuffers[imageIndex].CopyMemory(&i, sizeof(i));
}

Element::Light *Element::LightManager::getLight() {
    for (auto& light : lights)
    {
        if(!light){
            light = std::make_unique<Light>();
            return light.get();
        }
    }

    if(lights.size() >= MAX_LIGHT_COUNT)
        return nullptr;

    return lights.emplace_back(std::make_unique<Light>()).get();
}

std::vector<Element::Buffer> &Element::LightManager::getLightBuffers() {
    return lightBuffers;
}

std::vector<Element::Buffer> &Element::LightManager::getLightConstantsBuffers() {
    return lightConstantsBuffers;
}

void Element::LightManager::deInit() {

    for (int i = 0; i < lightBuffers.size(); ++i) {

        lightBuffers[i].Destroy();
        lightConstantsBuffers[i].Destroy();
    }
}
