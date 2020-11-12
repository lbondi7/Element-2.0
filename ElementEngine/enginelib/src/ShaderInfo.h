//
// Created by lbondi7 on 04/11/2020.
//

#pragma once

#include <element/ElementConstants.h>

#include <string>

namespace Element{

    enum class Descriptor : int{
        OBJECT = 2,
        CAMERA = 0,
        LIGHTING = 1
    };

    struct ShaderInfo {
        BindObjectType bindObjectType = BindObjectType::STATIC_UNIFORM_BUFFER;
        ShaderType shaderType = ShaderType::VERTEX;
        std::string shader = "shader";
        uint32_t set = 0;
        uint32_t binding = 0;
        uint32_t count = 1;
        //Descriptor descriptor = Descriptor::OBJECT;

        bool operator==(const ShaderInfo& other) const = default;
        ShaderInfo& operator=(const ShaderInfo& other) = default;
    };
}
 //VULKANTEST_DESCRIPTORINFO_H
