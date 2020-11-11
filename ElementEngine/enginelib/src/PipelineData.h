//
// Created by lbondi7 on 04/11/2020.
//

#pragma once

#include "Shader.h"
#include "ShaderInfo.h"

#include <element/GameSettings.h>

namespace Element{

struct PipelineData {
    std::vector<ShaderInfo> shaderInfo;

    bool depthEnabled = GameSettings::get().depthEnabled;
    int Topology = 0;

    bool operator==(const PipelineData& other) const = default;
    PipelineData& operator=(const PipelineData& other) = default;
};
}

 //VULKANTEST_PIPIELINEDATA_H
