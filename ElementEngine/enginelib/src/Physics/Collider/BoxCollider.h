//
// Created by lbondi7 on 01/12/2020.
//

#pragma once

#include <element/Transform.h>
#include <Resource/Mesh.h>
#include <VknBuffer.h>
#include <Pipeline/VknDescriptorSet.h>
#include <glm/glm.hpp>

namespace Element {
    class BoxCollider {
    public:
        BoxCollider() = default;
        ~BoxCollider() = default;

        void init(const glm::mat4* _objectTransform);

        bool intersect(const BoxCollider &other);

        void update(uint32_t imageIndex);

        VknDescriptorSet* getDescriptorSet();

        void initDescriptorSet(VknPipeline* pipeline, uint32_t imageCount);
        Mesh* mesh;
        Transform transform;

    private:
        std::vector<void *> data;
        std::vector<VknBuffer> uniformBuffers;
        std::unique_ptr<VknDescriptorSet> descriptorSet;

        const glm::mat4* objectTransform;
        glm::mat4 transformMatrix;
        Vec3 min;
        Vec3 max;
    };
}

//ELEMENTENGINE_BOXCOLLIDER_H
