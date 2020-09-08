#pragma once
#include "Registry.h"
#include "System.h"
#include "Decision.h"

namespace Systems {
	class MakeDecision : public System {
	public:
		void Update(float deltaTime) override
		{
			for (Entity entity : mEntities)
			{
				Components::DecisionTree& decisionTree = Registry::GetInstance()->GetComponent<Components::DecisionTree>(entity);
				
			/*	decisionTree.time += deltaTime;

				if (decisionTree.time >= decisionTree.interval)
				{
					decisionTree.time -= decisionTree.interval;*/
					decisionTree.root->mDecision(entity);
				//}
			}
		}
	};
}