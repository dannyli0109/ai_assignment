#pragma once
#include "Registry.h"
#include "System.h"
#include "Transform.h"
#include "RigidBody.h"

namespace Systems {
	class Physics : public System {
	public:
		// upldate position using velocity and accerleration
		void Update(float deltaTime) override
		{
			for (Entity entity : mEntities)
			{
				Components::Transform& transform = Registry::GetInstance()->GetComponent<Components::Transform>(entity);
				Components::RigidBody& rigidBody = Registry::GetInstance()->GetComponent<Components::RigidBody>(entity);

				rigidBody.velocity += rigidBody.accerleration * deltaTime;
				if (Math::Magnitude(rigidBody.velocity) > rigidBody.maxSpeed)
				{
					rigidBody.velocity = Math::Normalise(rigidBody.velocity) * rigidBody.maxSpeed;
				}
				//rigidBody.velocity = Math::Normalise(rigidBody.velocity) * rigidBody.maxSpeed;
				transform.position += rigidBody.velocity * deltaTime;
				rigidBody.accerleration *= 0;
			}
		}
	};
}