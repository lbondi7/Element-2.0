#pragma once

#include "Engine.h"
namespace Element {
	class Framework : public Engine
	{
	public:
		Framework() = default;
		~Framework() override;

	protected:
		void renderFrame() override final;

		void beginFrame() override final;

		void endFrame() override final;

	private:

		void start() override final;

	};
}