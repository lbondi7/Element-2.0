#pragma once

#include <element/Sprite.h>
#include <Vulkan/Pipeline/VknPipeline.h>
#include <Resource/Mesh.h>
#include <Texture/Texture.h>
#include <Vulkan/Pipeline/VknDescriptorSet.h>

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

        void updateUniformBuffers(uint32_t imageIndex);
        void setEntityState(EntityState state);
        EntityState getEntityState();
        EntityState getPrevEntityState();

        void destroy() override;

        void init(VknPipeline* pipeline, Mesh* mesh, uint32_t imageCount);

        void reInit(uint32_t imageCount);

        [[nodiscard]] VknDescriptorSet* getDescriptorSet() const;

    private:
        std::unique_ptr<VknDescriptorSet> descriptorSet;

        EntityState entityState;
        EntityState prevEntityState;

        VknPipeline* oldPipeline{};
        std::vector<VknBuffer> uniformBuffers;
        DirtyFlags dirty = DirtyFlags::CLEAN;

        glm::mat4 transformMatrix = glm::mat4(1.0f);
    public:
        const glm::mat4 &getTransformMatrix() const;
    };
}

