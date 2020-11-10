#include "Time.h"
#include <element/Debugger.h>

#include <iostream>
#include <algorithm>
#include <element/Time.h>

Element::Time::Time()
{
	startTimeStep = std::chrono::steady_clock::now();
	prevTimeStep = startTimeStep;
}

void Element::Time::updateDeltaTime()
{
	auto timepoint = std::chrono::steady_clock::now();
	delta = (timepoint - prevTimeStep);
	prevTimeStep = timepoint;
}

double Element::Time::getDelta()
{
	return (delta.count() / 1000.0) * timeScale;
}

float Element::Time::getDelta_f()
{
	return (static_cast<float>(delta.count()) / 1000.0f) * static_cast<float>(timeScale);
}

double Element::Time::getGameTime()
{
	return std::chrono::duration<double, std::milli>(std::chrono::steady_clock::now() - startTimeStep).count() / 1000.0;
}

float Element::Time::getGameTime_f()
{
	return std::chrono::duration<float, std::milli>(std::chrono::steady_clock::now() - startTimeStep).count() / 1000.0f;
}

int Element::Time::getGameTime_i()
{
	return static_cast<int>(std::chrono::duration<double, std::milli>(std::chrono::steady_clock::now() - startTimeStep).count() / 1000.0);
}

void Element::Time::setTimeScale(double _timeScale)
{
	timeScale = static_cast<double>(_timeScale);
}

void Element::Time::setTimeScale(float _timeScale)
{
	timeScale = static_cast<double>(_timeScale);
}

void Element::Time::setTimeScale(int _timeScale)
{
	timeScale = static_cast<double>(_timeScale);
}

double Element::Time::getTimeScale()
{
	return timeScale;
}

float Element::Time::getTimeScale_f()
{
	return static_cast<float>(timeScale);
}

int Element::Time::getTimeScale_i()
{
	return static_cast<int>(timeScale);
}

void Element::Time::updateFPS()
{
	static auto lastFPSCount = getGameTime();
	++frameCount;
	const auto elapsed = getGameTime() - lastFPSCount;
	if (elapsed >= 1.0)
	{
		fps = static_cast<int>(static_cast<double>(frameCount) / elapsed);
		frameCount = 0;
		lastFPSCount = getGameTime();
        Debugger::get().log("FPS", fps, Colour::BRIGHT_CYAN);
	}
}

int Element::Time::getFPS()
{
	return fps;
}

void Element::Time::getFPS(int& value)
{
	value = fps;
}

bool Element::Time::getFrame(int frameNumber)
{
	return frameCount % frameNumber == 0;
}

void Element::Time::setRenderFrameStep(int step)
{
	renderFrameStep = std::min(1, step);
}

int Element::Time::getRenderFrameStep()
{
	return renderFrameStep;
}
