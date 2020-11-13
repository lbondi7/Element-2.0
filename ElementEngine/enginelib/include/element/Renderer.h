#pragma once

#include "Camera.h"
#include "Window.h"

#include <memory>

namespace Element {

	class Model;
	struct Mesh;
	//class VknPipeline;
	struct Texture;
	class Sprite;

	class Renderer
	{
	public:

		Renderer() = default;
		virtual ~Renderer() = default;

		virtual void renderModel(Model* model) = 0;

		virtual void init() = 0;

		virtual Model* createModel() = 0;

		virtual Mesh* getMesh(const std::string& name) = 0;

		//virtual VknPipeline* getPipeline(const std::string& name) = 0;

		virtual Texture* getTexture(const std::string& name) = 0;

		virtual Element::Window* getWindow() = 0;

		virtual void deInit() = 0;

		virtual void signalExit() = 0;

		virtual void beginFrame() = 0;

		virtual void renderFrame() = 0;

		virtual void endFrame() = 0;

		virtual void setClearColour(const Vec3& clearCol) = 0;

		virtual void setClearColourNormalised(const Vec3& clearCol) = 0;

		virtual void addCamera(Element::Camera *_camera, int element) = 0;

		virtual Camera* createCamera(Element::CameraType type) = 0;

        virtual std::unique_ptr<Camera> createUniqueCamera(Element::CameraType type) = 0;

		virtual Sprite* createNewSprite() = 0;

		virtual void renderSprite(Sprite* sprite) = 0;

	protected:

		enum class RenderStage : int {
			START = 0,
			IMAGE_AQUIRED = 1,
			PRESENT_READY = 2,
			PRESENT_COMPLETE = 3
		};

		std::unique_ptr<Window> window;

		RenderStage renderStage = RenderStage::START;

		Vec3 clearColour = Vec3(0.0f, 0.0f, 1.0f);
	};
}