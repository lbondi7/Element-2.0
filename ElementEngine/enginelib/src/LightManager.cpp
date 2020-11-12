//
// Created by lbondi7 on 11/11/2020.
//

#include "LightManager.h"
#include "Locator.h"
#include "VknResources.h"

#include <element/Debugger.h>
#include <glm/glm.hpp>

const int MAX_LIGHT_COUNT = 100;

Element::LightManager::LightManager(uint32_t imageCount) {

    init(imageCount);

}

void Element::LightManager::init(uint32_t imageCount) {

    lightBuffers.resize(imageCount);
    lightConstantsBuffers.resize(imageCount);

    Vec3 pos = Vec3(0, 0, 0); //{Vec3(1, -1, 1), Vec3(0, 3, 0), Vec3(-3, 0, -1.5)};
    Vec3 col = Vec3(0, 1, 0); //{Vec3(1, 1, 1), Vec3(0.5, 0, 0), Vec3(0.2, 0, 1)};
    for (int i = 0; i < imageCount; ++i) {

        lightBuffers[i].Create(static_cast<VkDeviceSize>(sizeof(LightData) * MAX_LIGHT_COUNT), 0,
                               VK_BUFFER_USAGE_STORAGE_BUFFER_BIT);
        lightBuffers[i].Map();

        lightConstantsBuffers[i].Create(sizeof(LightConstants), 0, VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT);
        lightConstantsBuffers[i].Map();

    }
    lights.emplace_back(std::make_unique<Light>());
    lights[0]->setPosition(pos);
    lights[0]->setColour(col);
    lightCount++;



}


void Element::LightManager::update(const Vec3& camPos, uint32_t imageIndex){

    LightData l[MAX_LIGHT_COUNT];
    int i = 0;
    int position = 0;
    for(const auto& light : lights){
         if(!light) {
             free_position.emplace(position);
             ++position;
             continue;
         }

         if(!light->active) {
             ++position;
             continue;
         }

        l[i].pos = light->getPosition();
        l[i].direction = light->getDirection();
        l[i].type = static_cast<int>(light->getType());
        l[i].attenuation = Vec3(light->getConstant(), light->getLinear(), light->getQuadratic());
        l[i].col = light->getColour();
        l[i].intensity = light->getIntensity();
        //Debugger::get().log("Intensity", light->getIntensity());
        l[i].cutOff = glm::cos(glm::radians(light->getCutOff()));
        l[i].outerCutOff = glm::cos(glm::radians(light->getOuterCutOff()));

        l[i].pos.y *= -1;
        //l[i].col.y *= -1;
        //l[i].direction.y *= -1;

        ++i;
        ++position;
    }

    LightConstants lightConst{};
    //lightConst.camPos = camPos;
    //lightConst.camPos.y *= -1;
    lightConst.number = i;

    lightBuffers[imageIndex].CopyMemory(&l, sizeof(LightData) * i);
    lightConstantsBuffers[imageIndex].CopyMemory(&lightConst, sizeof(lightConst));
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

std::vector<std::unique_ptr<Element::Light>> &Element::LightManager::getLights() {
    return lights;
}
