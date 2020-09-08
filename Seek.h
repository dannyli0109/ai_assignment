#pragma once
#include "Registry.h"
#include <vector>

namespace Components
{
	struct Seek
	{
		std::vector<Entity> target;
		float maxSpeed;
	};
}