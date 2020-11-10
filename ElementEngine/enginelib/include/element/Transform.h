//
// Created by lbondi7 on 08/11/2020.
//

#pragma once

#include "element/Maths/Vec3.h"

namespace Element {
    struct Transform {
    public:
        bool operator==(const Transform& other) const;

        Transform &operator=(const Transform& other);

        [[nodiscard]] virtual const Element::Maths::Vec3& getPosition() const { return m_position; }
        [[nodiscard]] float getPositionX() const;
        [[nodiscard]] float getPositionY() const;
        [[nodiscard]] float getPositionZ() const;
        virtual void setPosition(const Element::Maths::Vec3& position);
        virtual void setPosition(float x, float y, float z) final;
        void setPositionX(float x);
        void setPositionY(float y);
        virtual void setPositionZ(float z);

        [[nodiscard]] virtual const Element::Maths::Vec3& getRotation() const;
        [[nodiscard]] float getRotationX() const;
        [[nodiscard]] float getRotationY() const;
        [[nodiscard]] float getRotationZ() const;
        virtual void setRotation(const Element::Maths::Vec3& rotation);
        virtual void setRotation(float x, float y, float z);
        virtual void setRotationX(float x);
        virtual void setRotationY(float y);
        void setRotationZ(float z);

        [[nodiscard]] const Element::Maths::Vec3& getScale() const;
        [[nodiscard]] float getScaleX() const;
        [[nodiscard]] float getScaleY() const;
        [[nodiscard]] float getScaleZ() const;
        virtual void setScale(const Element::Maths::Vec3& scale);
        virtual void setScale(float x, float y, float z);
        virtual void setScale(float scale);
        void setScaleX(float x);
        void setScaleY(float y);
        virtual void setScaleZ(float z);

        [[nodiscard]] const bool isUpdated() const { return updated; }

        bool updated = false;
    protected:
        Element::Maths::Vec3 m_position = Vec3(0.0f);
        Element::Maths::Vec3 m_rotation = Vec3(0.0f);
        Element::Maths::Vec3 m_scale = Vec3(1.0f);
    private:

    };

}

//ELEMENTENGINE_TRANSFORM_H
