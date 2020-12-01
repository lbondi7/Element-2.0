//
// Created by lbondi7 on 01/12/2020.
//

#include "Physics.h"

Element::BoxCollider *Element::Physics::getCollider(const glm::mat4* objectTransform) {

    for (auto& collider : colliders)
    {
        if(!collider){
            collider = std::make_unique<BoxCollider>();
            collider->init(objectTransform);
            return collider.get();
        }
    }
    auto& collider = colliders.emplace_back();
    collider = std::make_unique<BoxCollider>();
    collider->init(objectTransform);
    return collider.get();
}

const std::vector<std::unique_ptr<Element::BoxCollider>> &Element::Physics::getColliders() {
    return colliders;
}
