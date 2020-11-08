#pragma once

#include <glm/glm.hpp>

#include <vector>
#include <string>

namespace Element {

	class DebugWindow {

	public:

		enum class WindowType : int {
			CUSTOM = 0,
			DEMO = 1,
			ABOUT = 2
		};

		//enum class SectionType : int {
		//	TEXT = 0,
		//	INT = 1,
		//	FLOAT = 2,
		//	DOUBLE = 3,
		//	VEC_2 = 4,
		//	VEC_3 = 5,
		//	CHECK_BOX = 6,
		//	GRAPH = 7,
		//};

		DebugWindow(const std::string& label = "Debug", WindowType type = WindowType::CUSTOM);
		DebugWindow(WindowType type, const std::string& label = "Debug");
		~DebugWindow();


		void text(const std::string& label);
		void data(const std::string& label, int data);
		void graph(const std::string& label, void* data, float min, float max, int plotCount = 100, glm::vec2 size = glm::vec2(0, 80), const std::string& overlayText = "");
		void data(const std::string& label, float data);
		void checkBox(const std::string& label, bool* data);
		void data(const std::string& label, bool data);

		void end();

		void createChild(const std::string& label);
		void endChild(const std::string& label);


	private:
		std::string label;
		float zOrder = 1.0f;
		bool started = false;
	};
}