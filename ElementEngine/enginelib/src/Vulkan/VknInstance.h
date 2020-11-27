#pragma once

#include "VknConstants.h"

namespace Element {
	class Instance
	{
	public:
		Instance();
		~Instance();

		void Destroy();

		VkInstance GetVkInstance();

	private:

		void init();
		std::vector<const char*> getRequiredExtensions();
		bool checkValidationLayerSupport();
		VkInstance instance;

	};
}

