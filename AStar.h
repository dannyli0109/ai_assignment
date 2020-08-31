#pragma once
#include "Registry.h"
#include "System.h"
#include "Transform.h"
#include "RigidBody.h"
#include "AStarNode.h"
#include <vector>
#include <queue>

namespace Systems {
	class Physics : public System {
	public:
		void Update(float deltaTime) override
		{
			//for (Entity entity : mEntities)
			//{
			//	Components::Transform& transform = Registry::GetInstance()->GetComponent<Components::Transform>(entity);
			//	Components::RigidBody& rigidBody = Registry::GetInstance()->GetComponent<Components::RigidBody>(entity);
			//	Components::AStarNode& aStarNode = Registry::GetInstance()->GetComponent<Components::AStarNode>(entity);
			//	
			//}
		}

		std::vector<Components::AStarNode*> Search(Components::AStarNode* start, Components::AStarNode* end)
		{
			for (Entity entity : mEntities)
			{
				Components::Transform& transform = Registry::GetInstance()->GetComponent<Components::Transform>(entity);
				Components::RigidBody& rigidBody = Registry::GetInstance()->GetComponent<Components::RigidBody>(entity);
				Components::AStarNode& aStarNode = Registry::GetInstance()->GetComponent<Components::AStarNode>(entity);

				if (start == nullptr || end == nullptr) return std::vector<Components::AStarNode*>();
				if (start == end) return std::vector<Components::AStarNode*>();

				start->g = 0;
				start->previous = nullptr;

				std::priority_queue<Components::AStarNode*, std::vector<Components::AStarNode*>, GreaterThanByFScore> openList;
				std::unordered_map<Components::AStarNode*, bool> openListExisted;
				std::unordered_map<Components::AStarNode*, bool> closeList;

				openList.push(start);
				openListExisted[start] = true;


			}
		}
	};
}