#pragma once

#include "VknConstants.h"

namespace Element {


	struct ValidationLayers
	{
		static void populateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT& createInfo);
		static void CreateInfoDebugSetup(VkInstanceCreateInfo& createInfo);
		static VkInstanceCreateInfo CreateInfoDebugSetup();
		void setupDebugMessenger(VkInstance instance);

		void Destroy();

	private:

		VkInstance instance;

		VkDebugUtilsMessengerEXT m_debugMessenger;
	};
}

