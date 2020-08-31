#pragma once
#include "Vector3.h"

namespace Components
{
	struct RigidBody
	{
		Math::Vector3 velocity;
		Math::Vector3 accerleration;
		float maxSpeed;
	};
}