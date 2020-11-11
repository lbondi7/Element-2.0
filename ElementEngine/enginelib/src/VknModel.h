#pragma once

#include <element/Model.h>
#include "Buffer.h"
#include "DescriptorSet.h"

namespace Element {

    class VknModel final : public Model
            {
    public:
        VknModel();

        VknModel(VknPipeline *pipeline);

        VknModel(VknPipeline *pipeline, uint32_t imageCount);

        VknModel(VknPipeline *pipeline, uint32_t imageCount, std::vector<Buffer>& cameraBuffers);

        ~VknModel() override;

        Mesh *GetMesh() override;

        Texture *GetTexture() override;

        VknPipeline *GetPipeline() override;

        void SetMesh(Mesh *mesh) override;

        void SetTexture(Texture *texture) override;

        void SetPipeline(VknPipeline *pipeline) override;


        [[nodiscard]] const std::vector<Buffer> &GetUniformBuffers() const noexcept;

        std::vector<Buffer> &GetUniformBuffers() noexcept;

        DirtyFlags isDirty();

        void setDirty(DirtyFlags flag);

        void updateUniformBuffers(bool cameraChanged, const glm::mat4 &viewMatrix, const glm::mat4 &projMatrix,
                                  uint32_t imageIndex);

        void updateUniformBuffers(bool cameraChanged, UniformBufferObject &ubo, uint32_t imageIndex);

        void setEntityState(EntityState state);

        Element::EntityState getEntityState();

        Element::EntityState getPrevEntityState();

        void destroy();

        void init(VknPipeline *pipeline, uint32_t imageCount);

        void reInit(uint32_t imageCount);

        DescriptorSet* descriptorSet;


    private:
        std::vector<void*> data;
        std::vector<Buffer> uniformBuffers;
        EntityState entityState;
        EntityState prevEntityState;

        VknPipeline *oldPipeline;
        DirtyFlags dirty = DirtyFlags::CLEAN;

        void init(VknPipeline *_pipeline, uint32_t imageCount, std::vector<Buffer> &cameraBuffers);
    };
}
