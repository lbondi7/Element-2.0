#pragma once

#include "ElementConstants.h"


#include <glm/glm.hpp>

#include <string>
#include <vector>

struct GLFWwindow;

namespace Element {
	class Window
	{
	public:
		Window() = default;
		virtual ~Window() = default;

		const std::string& getTitle();
		bool getFramebufferResized();
		void setFramebufferResized(bool _framebufferResized);

		virtual GLFWwindow* GetGLFWWindow() = 0;
		virtual void close() = 0;
		virtual void setResizeCallback() = 0;
		virtual void setUserPointer(void* pointer) = 0;
		virtual void setCurrentMonitor(int _monitor) = 0;
		virtual void setMode(WindowMode _mode) = 0;
		virtual void setSize(int _width, int _height) = 0;
		virtual void setTitle(const std::string& _title) = 0;
		virtual glm::vec2 getSize() = 0;
		virtual glm::vec2 getPosition() = 0;
		virtual bool tooSmall() = 0;
		virtual int shouldClose() = 0;
		virtual void pollEvents() = 0;

	protected:

		int width;
		int height;
		int refreshRate;
		std::string title;
		int xpos;
		int ypos;
		WindowMode mode;
		int currentMonitor = 0;
		int monitorCount = 0;
		bool framebufferResized = false;
		int windowedWidth;
		int windowedHeight;

	};
}
