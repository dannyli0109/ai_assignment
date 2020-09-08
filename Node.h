#pragma once

#include <vector>
#include "Registry.h"

namespace Components
{
	struct Node
	{
		std::vector<Entity> edges;
		bool blocked = false;
		int previous = -1;
		float g;
		float h;
		float GetF() const { return g + h; };
	};
}

struct GreaterThanByFScore
{
	bool operator()(Entity a, Entity b) const
	{
		const Components::Node aNode = Registry::GetInstance()->GetComponent<Components::Node>(a);
		const Components::Node bNode = Registry::GetInstance()->GetComponent<Components::Node>(b);
		return aNode.GetF() > bNode.GetF();
	}
};