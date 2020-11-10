//
// Created by lbondi7 on 10/11/2020.
//

#pragma once

#include <element/Transform.h>
namespace Element {

    enum class LightType : int{
        POINT = 0,
        DIRECTIONAL = 1,
        SPOT = 2
    };

    class Light {

    public:

    private:

        Transform transform;
        LightType type = LightType::POINT;
        float intensity = 1.0f;
        Vec3 colour;
    };
}

//ELEMENTENGINE_LIGHT_H
