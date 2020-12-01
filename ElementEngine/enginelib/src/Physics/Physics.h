//
// Created by lbondi7 on 01/12/2020.
//

#pragma once

#include "Collider/BoxCollider.h"

namespace Element {
    class Physics {
    public:
        static Physics& get(){
            static Physics physics;
            return physics;
        }


        BoxCollider* getCollider(const glm::mat4* objectTransform);


        const std::vector<std::unique_ptr<BoxCollider>>& getColliders();

    private:
        Physics() = default;
        ~Physics() = default;

        std::vector<std::unique_ptr<BoxCollider>> colliders;
    };
}

//ELEMENTENGINE_PHYSICS_H
