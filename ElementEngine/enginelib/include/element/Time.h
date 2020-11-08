#pragma once

#include <chrono>
#include <cctype>

namespace Element {
	class Time
	{
	public:
		Time();
		~Time() = default;

		void updateDeltaTime();

		double getDelta();
		float getDelta_f();
		double getGameTime();
		float getGameTime_f();
		int getGameTime_i();

		void setTimeScale(double _timeScale);
		void setTimeScale(float _timeScale);
		void setTimeScale(int _timeScale);

		double getTimeScale();
		float getTimeScale_f();
		int getTimeScale_i();

		void updateFPS();

		int getFPS();

		void getFPS(int& value);

		bool getFrame(int frameNumber);

		void setRenderFrameStep(int step);

		int getRenderFrameStep();

	private:
		std::chrono::duration<double, std::milli> delta;
		std::chrono::steady_clock::time_point startTimeStep;
		std::chrono::steady_clock::time_point prevTimeStep;
		double timeScale = 1.0f;
		int fps = 0;
		int frameCount = 0;
		int renderFrameStep = 1;
	};
}
