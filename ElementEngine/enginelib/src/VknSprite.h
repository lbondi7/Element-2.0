#pragma once

#include <element/Sprite.h>
#include "VknPipeline.h"
#include "Mesh.h"
#include "Texture.h"
#include "DescriptorSet.h"

namespace Element {
    class VknSprite final :
        public Sprite
    {
    public:
        VknSprite() = default;
        explicit VknSprite(VknPipeline* pipeline, Mesh* mesh, uint32_t imageCount);

        ~VknSprite() override;

        Mesh* GetMesh();
        Texture* GetTexture() noexcept override;
        VknPipeline* GetPipeline() noexcept override;

        const Mesh* GetMesh() const override;
        const Texture* GetTexture() const noexcept override;
        const VknPipeline* GetPipeline() const noexcept override;

        void SetTexture(Texture* texture, bool keepSize) override;
        void SetPipeline(VknPipeline* pipeline) override;
        const VknPipeline* GetOldPipeline() const noexcept;

        DirtyFlags isDirty();
        void setDirty(DirtyFlags flag);

        void updateUniformBuffers(bool cameraChanged, const glm::mat4& viewMatrix, const glm::mat4& projMatrix, uint32_t imageIndex);

        void setEntityState(EntityState state);
        EntityState getEntityState();
        EntityState getPrevEntityState();

        void destroy();

        void init(VknPipeline* pipeline, Mesh* mesh, uint32_t imageCount);

        void reInit(uint32_t imageCount);

        //void setEntityState(EntityState state) override;
        //EntityState getEntityState() override;
        std::unique_ptr<DescriptorSet> descriptorSet;
    private:

        EntityState entityState;
        EntityState prevEntityState;

        VknPipeline* oldPipeline{};
        std::vector<Buffer> uniformBuffers;
        DirtyFlags dirty = DirtyFlags::CLEAN;
    };
}

