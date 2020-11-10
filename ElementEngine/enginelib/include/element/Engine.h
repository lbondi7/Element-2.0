#pragma once

#include "Renderer.h"
#include "Time.h"

#include <memory>

namespace Element {

	class Engine
	{
	public:
		void run();

		virtual void init() = 0;
		virtual void update(Time& epoch) = 0;
		virtual void render() = 0;

	protected:
		explicit Engine() = default;
		virtual ~Engine() = default;
		std::unique_ptr<Renderer> m_renderer;

	private:
		void beginFrame();
		void renderFrame();
		void endFrame();
		void start();
	};

}