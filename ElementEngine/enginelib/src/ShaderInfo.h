//
// Created by lbondi7 on 04/11/2020.
//

#pragma once

#include <element/ElementConstants.h>

#include <string>

namespace Element{
    struct ShaderInfo {
        BindObjectType bindObjectType = BindObjectType::STATIC_UNIFORM_BUFFER;
        ShaderType shaderType = ShaderType::VERTEX;
        std::string shader = "shader";
        uint32_t binding = 0;
        uint32_t count = 1;

        bool operator==(const ShaderInfo& other) const = default;
        ShaderInfo& operator=(const ShaderInfo& other) = default;
    };
}
 //VULKANTEST_DESCRIPTORINFO_H
