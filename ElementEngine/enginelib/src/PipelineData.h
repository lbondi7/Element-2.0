//
// Created by lbondi7 on 04/11/2020.
//

#pragma once

#include "Shader.h"
#include "DescriptorInfo.h"

namespace Element{

struct PipelineData {
    std::vector<DescriptorInfo> descriptorInfo;

    std::vector<Shader*> shaders;

    PipelineData& operator=(const PipelineData& other) {

        this->shaders = other.shaders;
        this->descriptorInfo = other.descriptorInfo;
        return *this;
    }
};
}

 //VULKANTEST_PIPIELINEDATA_H
