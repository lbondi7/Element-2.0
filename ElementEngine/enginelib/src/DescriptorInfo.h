//
// Created by lbondi7 on 04/11/2020.
//

#pragma once

#include <element/ElementConstants.h>
#include "Shader.h"
namespace Element{
    struct DescriptorInfo {
        DescriptorType descriptorType;
        Shader::ShaderType descriptorShader;
        uint32_t binding;

        DescriptorInfo& operator=(const DescriptorInfo& other) {
            this->descriptorType = other.descriptorType;
            this->binding = other.binding;
            this->descriptorShader = other.descriptorShader;
            return *this;
        }
    };
}
 //VULKANTEST_DESCRIPTORINFO_H
