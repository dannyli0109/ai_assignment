#pragma once
#include "AnimalStat.h"
#include "Seek.h"
#include "Tile.h"
#include "Decision.h"
#include "Registry.h"
#include <functional>
#include <vector>
#include "AStar.h"

class DecisionTree
{
public:
	DecisionTree(
		Entity entity,
		std::vector<std::vector<Entity>>& grid,
		float cellWidth, 
		float cellHeight,
		Systems::AStar* astar);
	~DecisionTree();
	Decision* mRoot;
	Entity mEntity;
	std::vector<std::vector<Entity>> mGrid;
	float mCellHeight;
	float mCellWidth;
	Systems::AStar* mAStar;

	std::function<void(void)> MakeDecision(Decision* decision, std::function<void(void)> func);

	void ResetAStarGrid();

};

