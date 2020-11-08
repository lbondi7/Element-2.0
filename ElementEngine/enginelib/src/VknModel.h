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

        ~VknModel() override;

        Mesh *GetMesh() override;

        Texture *GetTexture() override;

        VknPipeline *GetPipeline() override;

//        bool operator==(const VknModel &other) {
//            return this->mesh == other.mesh &&
//                   this->texture == other.texture &&
//                   this->pipeline == other.pipeline;
//        }

//		const Mesh* GetMesh() const noexcept override;
//		const Texture* GetTexture() const noexcept override;
//		const Pipeline* GetPipeline() const noexcept override;
//      const Pipeline* GetOldPipeline() const noexcept override;

        void SetMesh(Mesh *mesh) override;

        void SetTexture(Texture *texture) override;

        void SetPipeline(VknPipeline *pipeline) override;


        [[nodiscard]] const std::vector<Buffer> &GetUniformBuffers() const noexcept;

        std::vector<Buffer> &GetUniformBuffers() noexcept;

        DirtyFlags isDirty();

        void setDirty(DirtyFlags flag);

        void updateUniformBuffers(bool cameraChanged, const glm::mat4 &viewMatrix, const glm::mat4 &projMatrix,
                                  uint32_t imageIndex);

        void setEntityState(EntityState state);

        Element::EntityState getEntityState();

        Element::EntityState getPrevEntityState();

        void destroy();

        void init(VknPipeline *pipeline, uint32_t imageCount);

        void reInit(uint32_t imageCount);

        std::unique_ptr<DescriptorSet> descriptorSet;


    private:
        std::vector<Buffer> uniformBuffers;

        EntityState entityState;
        EntityState prevEntityState;

        VknPipeline *oldPipeline;
        DirtyFlags dirty = DirtyFlags::CLEAN;
    };
}
