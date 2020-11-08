#pragma once

#include <vector>
#include <set>

namespace Element {

	class IdHandler
	{
	public:

		static uint32_t GetID();

	private:

		static uint32_t idNumber;
		//static std::vector<uint32_t> currentIds;
		//static std::set<uint32_t> currentIds;

	};
}