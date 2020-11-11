#pragma once

#include "ElementConstants.h"

#include <string>

namespace Element {

	class GameSettings
	{
	public:

		enum class Dimension : int{
			_2D = 0,
			_3D = 1
		};

		enum class Multisample : int{
			MSAA_1 = 0,
			MSAA_2 = 1,
			MSAA_4 = 2,
			MSAA_8 = 3,
			MSAA_16 = 4,
			MSAA_32 = 5,
			MSAA_64 = 6,
		};

		enum class Pipelines : int {
		    STANDARD = 0,
		    LIGHTING = 1,
		    PBR = 2
		};

		static GameSettings& get(){
			static GameSettings gameSettings;
			return gameSettings;
		}

		int windowWidth = 800;
		int windowHeight = 600;
		std::string windowTitle = "Element Engine";
		WindowMode windowMode = WindowMode::WINDOWED;
		Dimension gameDimension = Dimension::_2D;
		Multisample msaaLevel = Multisample::MSAA_1;
		Pipelines pipelines = Pipelines::STANDARD;
		int fpsLimit = 60;
		bool depthEnabled = true;

	private:
		GameSettings() = default;
		~GameSettings() = default;
	};
}