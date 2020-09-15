#pragma once
#include "Registry.h"
#include <vector>
#include "Vector3.h"

struct Obstacle
{
	Entity entity;
	float radius;
};

namespace Components
{
	struct Avoid
	{
		std::vector<Obstacle*> obstacles;
		float maxSpeed;
		float maxSeeAhead;

		void ClearObstacles()
		{
			for (Obstacle* obstacle : obstacles)
			{
				delete obstacle;
			}
		}
	};
}