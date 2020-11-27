//
// Created by lbondi7 on 11/11/2020.
//

#include "LightManager.h"
#include <Misc/Locator.h>
#include <Vulkan/Pipeline/VknPipeline.h>

#include <element/Debugger.h>
#include <glm/glm.hpp>

const int MAX_LIGHT_COUNT = 100;

Element::LightManager::LightManager(Element::VknPipeline *pipeline, uint32_t imageCount) {

    init(pipeline, imageCount);

}

void Element::LightManager::init(Element::VknPipeline *pipeline, uint32_t imageCount) {

    lightBuffers.resize(imageCount);
    lightConstantsBuffers.resize(imageCount);

    Vec3 pos = Vec3(0, 0, 0); //{Vec3(1, -1, 1), Vec3(0, 3, 0), Vec3(-3, 0, -1.5)};
    Vec3 col = Vec3(0, 1, 0); //{Vec3(1, 1, 1), Vec3(0.5, 0, 0), Vec3(0.2, 0, 1)};
    for (int i = 0; i < imageCount; ++i) {

        Debugger::get().log("Light Buffer", Colour::BRIGHT_BLUE);
        lightBuffers[i].Create(static_cast<VkDeviceSize>(sizeof(LightData) * MAX_LIGHT_COUNT), 0,
                               VK_BUFFER_USAGE_STORAGE_BUFFER_BIT);
        lightBuffers[i].Map();
        Debugger::get().log("Light Constant", Colour::BRIGHT_BLUE);
        lightConstantsBuffers[i].Create(sizeof(LightConstants), 0, VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT);
        lightConstantsBuffers[i].Map();

    }
    lights.emplace_back(std::make_unique<Light>());
    lights[0]->setPosition(pos);
    lights[0]->setColour(col);
    lightCount++;

    addDescriptorSet(pipeline, imageCount);
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

std::vector<Element::VknBuffer> &Element::LightManager::getLightBuffers() {
    return lightBuffers;
}

std::vector<Element::VknBuffer> &Element::LightManager::getLightConstantsBuffers() {
    return lightConstantsBuffers;
}

void Element::LightManager::deInit() {

    for (auto& set : descriptorSets)
        set.second->flush();

    descriptorSets.clear();

    for (int i = 0; i < lightBuffers.size(); ++i) {
        lightBuffers[i].Destroy();
        lightConstantsBuffers[i].Destroy();
    }
}

std::vector<std::unique_ptr<Element::Light>> &Element::LightManager::getLights() {
    return lights;
}

void Element::LightManager::addDescriptorSet(Element::VknPipeline *pipeline, uint32_t imageCount) {

    const auto& name = pipeline->getName();
    descriptorSets.emplace(name, std::make_unique<VknDescriptorSet>(pipeline, imageCount, 1));
    descriptorSets[name]->addData(lightBuffers.data(), 0);
    descriptorSets[name]->addData(lightConstantsBuffers.data(), 1);
    descriptorSets[name]->createDescWritesAndUpdate();
}

Element::VknDescriptorSet *Element::LightManager::getDescriptorSet(const std::string &pipelineName) {
    return descriptorSets[pipelineName].get();
}
