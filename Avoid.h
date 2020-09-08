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

		/*Avoid(std::vector<Obstacle*> obstacles, float maxSpeed, float maxSeeAhead) : obstacles(obstacles), maxSpeed(maxSpeed), maxSeeAhead(maxSeeAhead) {}

		Avoid(const Avoid& other)
		{
			for (Obstacle* obstacle : other.obstacles)
			{
				Obstacle* newObstacle = new Obstacle{
					obstacle->center,
					obstacle->radius
				};

				obstacles.push_back(newObstacle);
			}
			maxSpeed = other.maxSpeed;
			maxSeeAhead = other.maxSeeAhead;
		}

		Avoid& operator=(const Avoid& other)
		{
			for (Obstacle* obstacle : obstacles)
			{
				delete obstacle;
			}

			for (Obstacle* obstacle : other.obstacles)
			{
				Obstacle* newObstacle = new Obstacle{
					obstacle->center,
					obstacle->radius
				};

				obstacles.push_back(newObstacle);
			}
			maxSpeed = other.maxSpeed;
			maxSeeAhead = other.maxSeeAhead;
			return *this;
		}

		~Avoid()
		{
			for (Obstacle* obstacle : obstacles)
			{
				delete obstacle;
			}
		}*/
	};
}