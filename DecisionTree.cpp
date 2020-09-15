#include "DecisionTree.h"

DecisionTree::DecisionTree(
	Entity entity,
	std::vector<std::vector<Entity>>& grid,
	float cellWidth, float cellHeight,
	Systems::AStar* astar) : mEntity(entity), mGrid(grid), mCellWidth(cellWidth), mCellHeight(cellHeight), mAStar(astar)
{
	mRoot = nullptr;
}

DecisionTree::~DecisionTree()
{
	mRoot->DeleteChildren();
}


std::function<void(void)> DecisionTree::MakeDecision(Decision* decision, std::function<void(void)> func)
{
	return[decision, func]() {
		func();
		if (decision->mCondition())
		{
			decision->mTrueBranch->mDecision();
		}
		else
		{
			decision->mFalseBranch->mDecision();
		}
	};
}

void DecisionTree::ResetAStarGrid()
{
	for (int i = 0; i < mGrid.size(); i++)
	{
		for (int j = 0; j < mGrid[i].size(); j++)
		{
			Entity cell = mGrid[i][j];
			Components::Node& node = Registry::GetInstance()->GetComponent<Components::Node>(cell);
			node.previous = -1;
		}
	}
}


