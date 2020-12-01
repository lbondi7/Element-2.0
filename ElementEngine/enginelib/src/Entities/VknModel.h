#pragma once

#include <element/Model.h>

#include <Vulkan/VknBuffer.h>
#include <Vulkan/Pipeline/VknDescriptorSet.h>
#include <Physics/Collider/BoxCollider.h>

namespace Element {

    class VknModel final : public Model
            {
    public:
        VknModel();

        VknModel(VknPipeline *pipeline);

        VknModel(VknPipeline *pipeline, uint32_t imageCount);

        ~VknModel() override;

        Mesh *GetMesh() override;

        Texture *GetTexture() override;

        VknPipeline *GetPipeline() override;

        void SetMesh(Mesh *mesh) override;

        void SetTexture(Texture *texture) override;

        void SetPipeline(VknPipeline *pipeline) override;


        [[nodiscard]] const std::vector<VknBuffer> &GetUniformBuffers() const noexcept;

        std::vector<VknBuffer> &GetUniformBuffers() noexcept;

        DirtyFlags isDirty();

        void setDirty(DirtyFlags flag);

//        void updateUniformBuffers(const glm::mat4 &viewMatrix, const glm::mat4 &projMatrix,
//                                  uint32_t imageIndex);

        void updateUniformBuffers(uint32_t imageIndex);

        void setEntityState(EntityState state);

        Element::EntityState getEntityState();

        Element::EntityState getPrevEntityState();

        void destroy() override;

        void init(VknPipeline *pipeline, uint32_t imageCount);

        void reInit(uint32_t imageCount);

        [[nodiscard]] VknDescriptorSet* getDescriptorSet() const;

        [[nodiscard]] const glm::mat4& getTransformMatrix() const;

        BoxCollider* collider;
    private:
        std::vector<void*> data;
        std::vector<VknBuffer> uniformBuffers;
        EntityState entityState;
        EntityState prevEntityState;

        std::unique_ptr<VknDescriptorSet> descriptorSet;

        VknPipeline *oldPipeline;
        DirtyFlags dirty = DirtyFlags::CLEAN;

        glm::mat4 transformMatrix = glm::mat4(1.0f);
    };
}
