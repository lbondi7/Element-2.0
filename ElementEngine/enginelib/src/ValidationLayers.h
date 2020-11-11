#pragma once

#include "VknConstants.h"

namespace Element {

    const std::vector<const char*> validationLayers = {
            "VK_LAYER_KHRONOS_validation",
            "VK_LAYER_LUNARG_monitor"
    };

#ifdef NDEBUG
    const bool enableValidationLayers = false;
#else
    const bool enableValidationLayers = true;
#endif


	struct ValidationLayers
	{
	    static ValidationLayers& get(){
	        static ValidationLayers validationLayers;
            return validationLayers;
	    }

		void populateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT& createInfo);
		void setupDebugMessenger(VkInstance instance);

		void Destroy();

	private:
        ValidationLayers() = default;
        ~ValidationLayers() = default;

		VkInstance instance;

		VkDebugUtilsMessengerEXT m_debugMessenger;
	};
}

