#pragma once

#include <element/Window.h>

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

namespace Element {

    class GlfwWindow final :
        public Window
    {
	public:
		GlfwWindow();
		~GlfwWindow() override;

		void close() override;
		void setResizeCallback() override;
		void setUserPointer(void* pointer) override;
		GLFWwindow* GetGLFWWindow() override;
		Vec2 getSize() override;
		Vec2 getPosition() override;
		void setCurrentMonitor(int _monitor) override;
		void setMode(WindowMode _mode) override;
		void setSize(int _width, int _height) override;
		void setTitle(const std::string& _title) override;
		bool tooSmall() override;
		int shouldClose() override;
		void pollEvents() override;

	private:
		static void framebufferResizeCallback(GLFWwindow* window, int width, int height);

		struct VknMonitor : public Monitor {
			GLFWmonitor* glfwMonitor;
		};

		GLFWwindow* glfwWindow;
		GLFWmonitor** glfwMonitors;

		std::vector<VknMonitor> monitors;
    };
}
