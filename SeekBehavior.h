#pragma once

#include "Registry.h"
#include "System.h"
#include "Transform.h"
#include "Seek.h"
#include "RigidBody.h"
#include <iostream>

namespace Systems {
	class SeekBehavior : public System {
	public:
		// seek the list of positions
		void Update(float deltaTime) override
		{
			for (Entity entity : mEntities)
			{
				Components::Seek& behavior = Registry::GetInstance()->GetComponent<Components::Seek>(entity);
				Components::RigidBody& rigidBody = Registry::GetInstance()->GetComponent<Components::RigidBody>(entity);
				Components::Transform& transform = Registry::GetInstance()->GetComponent<Components::Transform>(entity);

				while (behavior.target.size() > 0)
				{
					const Components::Transform& targetPosition = Registry::GetInstance()->GetComponent<Components::Transform>(behavior.target[behavior.target.size() - 1]);

					float dist = Math::Magnitude(targetPosition.position - transform.position);
					if (dist > 40.0f)
					{
						Math::Vector3 desireVelocity = Math::Normalise(targetPosition.position - transform.position) * behavior.maxSpeed;
						Math::Vector3 steeringForce = desireVelocity - rigidBody.velocity;
						rigidBody.accerleration += steeringForce;
						break;
					}
					else {
						behavior.target.pop_back();
						if (behavior.target.size() == 0)
						{
							rigidBody.velocity *= 0;
						}
					}
				}
			}
		}

		void Debug()
		{
			for (Entity entity : mEntities)
			{
				Components::Seek& behavior = Registry::GetInstance()->GetComponent<Components::Seek>(entity);
				if (behavior.target.size() > 0)
				{
					for (int i = 0; i < behavior.target.size() - 1; i++)
					{
						Entity a = behavior.target[i];
						Entity b = behavior.target[i + 1];
						Components::Transform& aTransform = Registry::GetInstance()->GetComponent<Components::Transform>(a);
						Components::Transform& bTransform = Registry::GetInstance()->GetComponent<Components::Transform>(b);

						/*DrawLine(aTransform.position.x, aTransform.position.y, bTransform.position.x, bTransform.position.y, WHITE);*/

						DrawLineEx({ aTransform.position.x, aTransform.position.y }, { bTransform.position.x, bTransform.position.y }, 5, WHITE);
					}
				}
			}
		}
	};
}