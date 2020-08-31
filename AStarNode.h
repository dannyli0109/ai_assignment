#pragma once
#include <vector>
namespace Components
{
	struct AStarNode
	{
		std::vector<AStarNode*> edges;
		AStarNode* previous;
		bool blocked = false;
		float g;
		float h;
		float GetF() const { return g + h; };
	};
}

struct GreaterThanByFScore
{
	bool operator()(Components::AStarNode* a, Components::AStarNode* b) const
	{
		return a->GetF() > b->GetF();
	}
};