#pragma once

#include "Registry.h"
#include "System.h"
#include "Transform.h"
#include "Avoid.h"
#include "RigidBody.h"
#include <iostream>

namespace Systems {
	class AvoidBehavior : public System {
	public:
		
		void Update(float deltaTime) override
		{
			for (Entity entity : mEntities)
			{
				Components::Avoid& behavior = Registry::GetInstance()->GetComponent<Components::Avoid>(entity);
				Components::RigidBody& rigidBody = Registry::GetInstance()->GetComponent<Components::RigidBody>(entity);
				Components::Transform& transform = Registry::GetInstance()->GetComponent<Components::Transform>(entity);

				Math::Vector3 ahead = transform.position + Math::Normalise(rigidBody.velocity) * behavior.maxSeeAhead;
				Obstacle* mostThreatening = FindMostThreateningObstacle(entity);
				Math::Vector3 avoidance = Math::Vector3(0.0f, 0.0f, 0.0f);
				if (mostThreatening)
				{
					Components::Transform& t = Registry::GetInstance()->GetComponent<Components::Transform>(mostThreatening->entity);

					avoidance.x = ahead.x - t.position.x;
					avoidance.y = ahead.y - t.position.y;

					avoidance = Math::Normalise(avoidance);
					avoidance *= behavior.maxSpeed;
				}
				else
				{
					avoidance *= 0;
				}

				rigidBody.accerleration += avoidance;
			}
		}

		void Debug()
		{
			for (Entity entity : mEntities)
			{
				Components::Avoid& behavior = Registry::GetInstance()->GetComponent<Components::Avoid>(entity);
				//Components::RigidBody& rigidBody = Registry::GetInstance()->GetComponent<Components::RigidBody>(entity);
				//Components::Transform& transform = Registry::GetInstance()->GetComponent<Components::Transform>(entity);

				for (Obstacle* obstacle : behavior.obstacles)
				{
					Components::Transform& transform = Registry::GetInstance()->GetComponent<Components::Transform>(obstacle->entity);
					DrawCircleLines(transform.position.x,transform.position.y, obstacle->radius, WHITE);
				}
				//DrawCircleLines()
			}
		}

	private:
		bool IntersectObstacle(Entity entity, Obstacle* obstacle)
		{
			Components::Avoid& behavior = Registry::GetInstance()->GetComponent<Components::Avoid>(entity);
			Components::RigidBody& rigidBody = Registry::GetInstance()->GetComponent<Components::RigidBody>(entity);
			Components::Transform& transform = Registry::GetInstance()->GetComponent<Components::Transform>(entity);

			Math::Vector3 E = transform.position;
			Math::Vector3 L = transform.position + Math::Normalise(rigidBody.velocity) * behavior.maxSeeAhead;
			Components::Transform& obstacleTransform = Registry::GetInstance()->GetComponent<Components::Transform>(obstacle->entity);
			Math::Vector3 C = obstacleTransform.position;
			float r = obstacle->radius;

			// direction
			Math::Vector3 d = L - E;
			// cetner to ray start
			Math::Vector3 f = E - C;

			float a = Math::Dot(d, d);
			float b = 2 * Math::Dot(f, d);
			float c = Math::Dot(f, f) - r * r;

			float discriminant = b * b - 4 * a * c;

			if (discriminant < 0)
			{
				// no intersection
			}
			else
			{
				discriminant = sqrt(discriminant);
				float t1 = (-b - discriminant) / (2 * a);
				float t2 = (-b + discriminant) / (2 * a);

				if (t1 >= 0 && t1 <= 1)
				{
					// t1 is the intersection, and it's closer than t2
					// (since t1 uses -b - discriminant)
					// Impale, Poke
					return true;
				}

				// here t1 didn't intersect so we are either started
				// inside the sphere or completely past it
				if (t2 >= 0 && t2 <= 1)
				{
					// ExitWound
					return true;
				}
			}
			return false;
		}

		Obstacle* FindMostThreateningObstacle(Entity entity)
		{
			Obstacle* mostThreatening = nullptr;
			Components::Avoid& behavior = Registry::GetInstance()->GetComponent<Components::Avoid>(entity);
			Components::RigidBody& rigidBody = Registry::GetInstance()->GetComponent<Components::RigidBody>(entity);
			Components::Transform& transform = Registry::GetInstance()->GetComponent<Components::Transform>(entity);

			for (Obstacle* obstacle : behavior.obstacles)
			{
				Components::Transform& obstacleTransform = Registry::GetInstance()->GetComponent<Components::Transform>(obstacle->entity);

				bool collision = IntersectObstacle(entity, obstacle);
				if (
					collision && (
						mostThreatening == nullptr ||
						Math::Magnitude(transform.position - obstacleTransform.position) <
						Math::Magnitude(transform.position - Registry::GetInstance()->GetComponent<Components::Transform>(mostThreatening->entity).position)
						)
					) {
					mostThreatening = obstacle;
				}
			}
			return mostThreatening;
		}

	};
}