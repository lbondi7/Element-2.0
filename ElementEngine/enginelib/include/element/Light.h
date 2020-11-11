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

        Light() = default;
        ~Light() = default;


        [[nodiscard]] const Vec3& getPosition() const;

        void setPosition(const Vec3 &position);

        void setPosition(float x, float y, float z);

        void setPosX(float x);

        void setPosY(float y);

        void setPosZ(float y);

        LightType getType() const;

        void setType(LightType type);

        float getIntensity() const;

        void setIntensity(float intensity);

        [[nodiscard]] const Vec3 &getColour() const;

        void setColour(const Vec3 &colour);

        void setColour(float r, float g, float b);

        void setColR(float r);

        void setColG(float g);

        void setColB(float b);

        [[nodiscard]] const Vec3 &getDirection() const;

        void setDirection(const Vec3 &direction);

        void setDirection(float x, float y, float z);

        void setDirX(float x);

        void setDirY(float y);

        void setDirZ(float z);

        float getCutOff() const;

        void setCutOff(float cutOff);

        float getOuterCutOff() const;

        void setOuterCutOff(float outerCutOff);

        float getConstant() const;

        void setConstant(float constant);

        float getLinear() const;

        void setLinear(float linear);

        float getQuadratic() const;

        void setQuadratic(float quadratic);

    private:

        Transform transform;
        LightType type = LightType::POINT;

        float intensity = 1.0f;
        Vec3 colour;
        Vec3 direction = Vec3(1);
        float cutOff = 0.0f;
        float outerCutOff = 0.0f;
        float constant = 0.0f;
        float linear = 0.0f;
        float quadratic = 0.0f;
    };
}

//ELEMENTENGINE_LIGHT_H
