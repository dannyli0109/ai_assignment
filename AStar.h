#pragma once
#include "raylib.h"

#include "Registry.h"
#include "System.h"
#include "Transform.h"
#include "RigidBody.h"
#include "Node.h"
#include <vector>
#include <queue>

namespace Systems {
	class AStar : public System {
	public:

		std::vector<Entity> Search(Entity start, Entity end)
		{

			if (start == end) return std::vector<Entity>();
			Components::Node& startNode = Registry::GetInstance()->GetComponent<Components::Node>(start);
			Components::Node& endNode = Registry::GetInstance()->GetComponent<Components::Node>(end);
			Components::Transform& endTransform = Registry::GetInstance()->GetComponent<Components::Transform>(end);
			startNode.g = 0;
			startNode.previous = -1;

			std::priority_queue<Entity, std::vector<Entity>, GreaterThanByFScore> openList;
			std::unordered_map<Entity, bool> openListExisted;
			std::unordered_map<Entity, bool> closeList;

			openList.push(start);
			openListExisted[start] = true;

			while (!openList.empty())
			{
				Entity current = openList.top();
				if (current == end) break;

				openList.pop();
				openListExisted[current] = false;

				closeList[current] = true;

				Components::Node& currentNode = Registry::GetInstance()->GetComponent<Components::Node>(current);
				Components::Transform& currentTransform = Registry::GetInstance()->GetComponent<Components::Transform>(current);
				for (Entity e : currentNode.edges)
				{
					Components::Node& edgeNode = Registry::GetInstance()->GetComponent<Components::Node>(e);
					Components::Transform& edgeTransform = Registry::GetInstance()->GetComponent<Components::Transform>(e);
					if (!closeList[e])
					{
						float g = currentNode.g + Math::Magnitude(currentTransform.position - edgeTransform.position);
						float xDiff = abs(endTransform.position.x - edgeTransform.position.x);
						float yDiff = abs(endTransform.position.y - edgeTransform.position.y);
						float smallest = fmin(xDiff, yDiff);
						float biggest = fmax(xDiff, yDiff);
						float h = smallest * sqrt(2) + biggest - smallest;
						float f = g + h;

						if (!openListExisted[e])
						{
							edgeNode.g = g;
							edgeNode.h = h;
							edgeNode.previous = current;
							openList.push(e);
							openListExisted[e] = true;
						}
						else if (f < edgeNode.GetF())
						{
							edgeNode.g = g;
							edgeNode.h = h;
							edgeNode.previous = current;
						}

					}
				}
			}
			std::vector<Entity> path;
			int current = end;
			while (current > -1)
			{
				path.push_back(current);
				Components::Node& currentNode = Registry::GetInstance()->GetComponent<Components::Node>(current);
				current = currentNode.previous;
			}
			return path;
		}


		void Debug() const
		{
			for (Entity entity : mEntities)
			{
				Components::Transform& transform = Registry::GetInstance()->GetComponent<Components::Transform>(entity);
				Components::Node& aStarNode = Registry::GetInstance()->GetComponent<Components::Node>(entity);

				for (int i = 0; i < aStarNode.edges.size(); i++)
				{
					Entity otherEntity = aStarNode.edges[i];
					Components::Transform& otherTransform = Registry::GetInstance()->GetComponent<Components::Transform>(otherEntity);

					DrawLineEx({ transform.position.x, transform.position.y }, { otherTransform.position.x, otherTransform.position.y }, 5, BLUE);

				}
			}

			for (Entity entity : mEntities)
			{
				Components::Transform& transform = Registry::GetInstance()->GetComponent<Components::Transform>(entity);

				DrawCircle(transform.position.x, transform.position.y, 5, WHITE);
			}

			for (Entity entity : mEntities)
			{
				Components::Node& node = Registry::GetInstance()->GetComponent<Components::Node>(entity);
				Components::Transform& transform = Registry::GetInstance()->GetComponent<Components::Transform>(entity);
				if (node.previous > -1)
				{
					DrawCircle(transform.position.x, transform.position.y, 5, BLACK);
				}
			}
		}
	};
}