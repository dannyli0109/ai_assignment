#pragma once
#include "Registry.h"
#include "System.h"
#include "AnimalStat.h"
#include "raylib.h"
#include "Transform.h"
#include "Rect.h"
#include <sstream>

namespace Systems {
	class Animal : public System {
	public:
		void Update(float deltaTime) override
		{
			for (Entity entity : mEntities)
			{
				Components::AnimalStat& animalStat = Registry::GetInstance()->GetComponent<Components::AnimalStat>(entity);
				
				if (animalStat.drinking)
				{
					animalStat.thirst += animalStat.incrementRate * deltaTime;
					if (animalStat.thirst > 100.0f)
					{
						animalStat.thirst = 100.0f;
					}
				}
				else 
				{
					animalStat.thirst -= animalStat.decayRate * deltaTime;
				}

				if (animalStat.eating)
				{
					animalStat.hunger += animalStat.incrementRate * deltaTime;
					if (animalStat.hunger > 100.0f)
					{
						animalStat.hunger = 100.0f;
					}
				}
				else
				{
					animalStat.hunger -= animalStat.decayRate * deltaTime;
				}
			}
		}

		void Debug()
		{
			for (Entity entity : mEntities)
			{
				Components::Transform& transform = Registry::GetInstance()->GetComponent<Components::Transform>(entity);
				Components::Rect& rect = Registry::GetInstance()->GetComponent<Components::Rect>(entity);
				Components::AnimalStat& animalStat = Registry::GetInstance()->GetComponent<Components::AnimalStat>(entity);
				//std::stringstream thristStr;
				//thristStr << "Thirst: " << int(animalStat.thirst);
			/*	std::stringstream hungerStr;
				hungerStr << "Hunger: " << int(animalStat.hunger);

				DrawText(thristStr.str().c_str(), transform.position.x - rect.offX, transform.position.y - rect.offY - 24, 24, WHITE);*/
				


				DrawText(animalStat.decision.c_str(), transform.position.x - rect.offX, transform.position.y - 50, 24, WHITE);
			}
		}
	};
}

class Animal
{
};
