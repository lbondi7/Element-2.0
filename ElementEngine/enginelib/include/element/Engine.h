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

		virtual void beginFrame() = 0;
		virtual void renderFrame() = 0;
		virtual void endFrame() = 0;
		virtual void start() = 0;

		std::unique_ptr<Renderer> m_renderer;


	private:
	};

}