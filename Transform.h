#pragma once
#include "Vector3.h"

namespace Components
{
	struct Transform
	{
		Math::Vector3 position;
		float rotation;
		float scale;
	};
};