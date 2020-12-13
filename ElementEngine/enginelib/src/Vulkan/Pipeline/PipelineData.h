//
// Created by lbondi7 on 04/11/2020.
//

#pragma once

#include <Shader/Shader.h>
#include <Vulkan/Pipeline/DescriptorInfo.h>

#include <element/GameSettings.h>

namespace Element{

struct PipelineData {
    std::vector<DescriptorInfo> descriptorInfo;

    bool depthEnabled = GameSettings::get().depthEnabled;
    PipelinePolygonMode polygonMode = PipelinePolygonMode::FILL;
    PipelineCulling cullMode = PipelineCulling::BACK;
    bool lightingEnabled = true;

    bool operator==(const PipelineData& other) const = default;
    PipelineData& operator=(const PipelineData& other) = default;
};
}

 //VULKANTEST_PIPIELINEDATA_H
