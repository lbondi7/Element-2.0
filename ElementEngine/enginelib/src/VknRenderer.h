#pragma once

#include <element/Renderer.h>

#include "VknModel.h"
#include "VknSprite.h"
#include "VkFunctions.h"
#include "VknSwapChain.h"
#include "VknRenderPass.h"
#include "CommandPool.h"
#include "Resources.h"
#include "Image.h"
#include "Buffer.h"
#include "Device.h"
#include "ValidationLayers.h"
#include "CommandBuffer.h"
#include "ShaderManager.h"
#include "VknInstance.h"
#include "VknWindow.h"
#include "PipelineManager.h"
#include "DebugRenderer.h"

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

		void UnbindMeshes();

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

		void loadTexture(const std::string& texture) override;

		void setClearColour(const glm::vec3& clearCol) override;

		void setClearColourNormalised(const glm::vec3& clearCol) override;

	private:
		const int MAX_FRAMES_IN_FLIGHT = 2;

		std::unique_ptr<Instance> instance;
		VkSurfaceKHR surface;
		ValidationLayers debugLayers;

		std::unique_ptr<SwapChain> swapChain;
		std::unique_ptr<RenderPass> renderPass;
		std::unique_ptr<PipelineManager> m_pipelineManager;
		std::unique_ptr<ShaderManager> m_shaderManager;
		std::unique_ptr<CommandPool> graphicsCommandPool;

		std::vector<std::unique_ptr<CommandBuffer>> commandBuffers;

		std::vector<VkSemaphore> graphicsSemaphores;
		std::vector<VkSemaphore> presentSemaphores;
		std::vector<VkFence> inFlightFences;
		std::vector<VkFence> imagesInFlight;
		uint32_t currentFrame = 0;

		std::map<std::string, std::unique_ptr<Mesh>> meshes;
		std::map<std::string, std::unique_ptr<Texture>> texs;
		std::vector<std::unique_ptr<VknSprite>> vknSprites;
		std::vector<std::unique_ptr<VknModel>> vknModels;
		bool rebuildCmdBuffers = false;

		std::unique_ptr<DebugRenderer> debugRenderer;
	};
}