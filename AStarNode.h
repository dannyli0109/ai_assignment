#pragma once
#include <vector>


struct Node
{
	std::vector<Node*> edges;
	Node* previous = nullptr;
	bool blocked = false;
	float g;
	float h;
	float GetF() const { return g + h; };
};

struct GreaterThanByFScore
{
	bool operator()(Node* a, Node* b) const
	{
		return a->GetF() > b->GetF();
	}
};

namespace Components
{
	
}