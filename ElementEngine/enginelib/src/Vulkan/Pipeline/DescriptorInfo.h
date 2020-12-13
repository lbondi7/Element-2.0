//
// Created by lbondi7 on 04/11/2020.
//

#pragma once

#include <element/ElementConstants.h>

#include <string>

namespace Element{

    struct DescriptorInfo {
        BindObjectType bindObjectType = BindObjectType::STATIC_UNIFORM_BUFFER;
        ShaderType shaderType = ShaderType::VERTEX;
        std::string shader = "shader";
        uint32_t set = 0;
        uint32_t binding = 0;

        bool operator==(const DescriptorInfo& other) const = default;
        DescriptorInfo& operator=(const DescriptorInfo& other) = default;
    };
}
 //VULKANTEST_DESCRIPTORINFO_H
