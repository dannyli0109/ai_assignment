#pragma once
#include <unordered_map>
#include <functional>

namespace Components
{
	struct State
	{
		std::unordered_map<const char*, std::function<bool(void)>> map;
	};
}