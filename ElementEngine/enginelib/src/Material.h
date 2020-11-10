//
// Created by lbondi7 on 09/11/2020.
//

#pragma once

#include <element/Maths/Vec3.h>

namespace Element {

    struct MaterialData{
        Vec3 ambient;
        Vec3 diffuse;
        Vec3 specular;
    };

    struct Material {

        MaterialData data;

    };
}

//ELEMENTENGINE_MATERIAL_H
