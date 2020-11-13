#pragma once

#include <element/Renderer.h>

#include "VknModel.h"
#include "VknSprite.h"
#include "VknSwapChain.h"
#include "VknRenderPass.h"
#include "CommandPool.h"
#include "CommandBuffer.h"
#include "VknInstance.h"
#include "VknWindow.h"
#include "PipelineManager.h"
#include "DebugRenderer.h"
#include "LightManager.h"
#include "EleCamera.h"

namespace Element {

	class VknRenderer final : public Renderer
	{
	public:

		VknRenderer();
		~VknRenderer() override;

		void createSurface();

		void destroySurface();

		void init() override;

		void deInit() override;

		void beginFrame() override;
		void renderFrame() override;
		void endFrame() override;

		void recreateRenderer();

		void signalExit() override;

		void createRenderer();

		void cleanupSwapChain();

		void cleanupRenderer();

		void rebuildCommandBuffers();

		void BindMesh(Element::Mesh* mesh, VkCommandBuffer commandBuffer);

		void createSyncObjects();

		void renderModel(Model* object) override;

		void renderSprite(Sprite* sprite) override;

		void CheckModels();

		void CheckSprites();

		void updateUniformBuffers();

		Mesh* getMesh(const std::string& name) override;

		Texture* getTexture(const std::string& name) override;

//		VknPipeline* getPipeline(const std::string& name) override;

		Window* getWindow() override;

		Model* createModel() override;

		Sprite* createNewSprite() override;

		Camera* createCamera(Element::CameraType type) override;

        std::unique_ptr<Camera> createUniqueCamera(Element::CameraType type) override;

		void setClearColour(const Vec3& clearCol) override;

		void setClearColourNormalised(const Vec3& clearCol) override;

        void addCamera(Element::Camera *_camera, int element) override;

	private:
		const int MAX_FRAMES_IN_FLIGHT = 2;

		std::unique_ptr<Instance> instance;
		VkSurfaceKHR surface;

		std::unique_ptr<SwapChain> swapChain;
		std::unique_ptr<RenderPass> renderPass;
		std::unique_ptr<PipelineManager> m_pipelineManager;
		std::unique_ptr<CommandPool> graphicsCommandPool;

		std::vector<std::unique_ptr<CommandBuffer>> commandBuffers;

		std::vector<VkSemaphore> graphicsSemaphores;
		std::vector<VkSemaphore> presentSemaphores;
		std::vector<VkFence> inFlightFences;
		std::vector<VkFence> imagesInFlight;
		uint32_t currentFrame = 0;

		std::vector<std::unique_ptr<VknSprite>> vknSprites;
		std::vector<std::unique_ptr<VknModel>> vknModels;
		bool rebuildCmdBuffers = false;

		std::unique_ptr<DebugRenderer> debugRenderer;

		std::unique_ptr<LightManager> m_lightManager;

		std::vector<EleCamera*> cameras;

//		std::vector<Buffer> camBuffers;
//
//		DescriptorSet* camDescSet;

		int type = 0;
		float pos[3] = {0, 0, 0};
        float col[3] = {1, 1, 1};
        float dir[3] = {1, 1, 1};
        float attenuation[3] = {1.0f, 0.09f, 0.032};
        float intensity = 1.0f;
        float cutOff = 12.5;
        float outerCutOff = 15.0f;

        void rebuildCommandBuffersTemp();
    };
}