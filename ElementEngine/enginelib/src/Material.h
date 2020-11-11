//
// Created by lbondi7 on 09/11/2020.
//

#pragma once

#include <element/Maths/Vec3.h>
#include "Buffer.h"

namespace Element {

    struct MaterialData{
        Vec3 ambient;
        Vec3 diffuse;
        Vec3 specular;
        Vec3 transmittance;
        Vec3 emission;
        float shininess;
        float refractionIndex;
        float dissolve;
        float illum;

        /// Physically Based Rendering values
        float roughness;
        float metallic;
        float sheen;
        float anisotropy;
    };

    struct Material {

        MaterialData data;

        std::vector<Buffer> uniformBuffers;

        void destroy();
        void load();

    };
}

//ELEMENTENGINE_MATERIAL_H
