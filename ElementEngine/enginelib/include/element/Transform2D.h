//
// Created by lbondi7 on 08/11/2020.
//

#pragma once

#include "Transform.h"
#include "element/Maths/Vec2.h"

namespace Element {
    struct Transform2D : public Element::Transform {
    public:

        bool operator==(const Transform2D& other);

        Transform2D& operator=(const Transform2D& other);

        Element::Maths::Vec2 getPosition2D();
        void setPosition(Element::Maths::Vec2 position);
        void setPosition(const Element::Maths::Vec3& position) override;
        void setPosition(float x, float y);

        Element::Maths::Vec2 getSize();
        [[nodiscard]] float getSizeX() const;
        [[nodiscard]] float getSizeY() const;
        void setSize(Element::Maths::Vec2 size);
        void setSize(float x, float y);
        void setSizeX(float x);
        void setSizeY(float y);

        Element::Maths::Vec2 getScale2D();
        void setScale(const Element::Maths::Vec3& scale) override;
        void setScale(float x, float y, float z) override;
        void setScale(float scale) override;
        void setScale(Element::Maths::Vec2 scale);
        void setScale(float x, float y);
        void setScaleZ(float z) override;

        void setRotation(float rotation);
        void setRotation(const Element::Maths::Vec3& rotation) override;
        void setRotation(float x, float y, float z) override;
        void setRotationX(float x) override;
        void setRotationY(float y) override;

    private:
        Element::Maths::Vec2 m_size = Vec2(0.0f);
    };
}

//ELEMENTENGINE_TRANSFORM2D_H
