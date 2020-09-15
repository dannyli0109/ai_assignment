#pragma once
#include "DecisionTree.h"
#include <vector>
class LlamaDecisionTree :
	public DecisionTree
{
public:
	LlamaDecisionTree(
		Entity entity,
		std::vector<std::vector<Entity>>& grid,
		float cellWidth,
		float cellHeight,
		Systems::AStar* astar,
		std::vector<Entity>& preys
	);

	std::vector<Entity>& mPreys;

	void Init();

	// To drink
	Decision* ToDrink();
	bool IsDrinking();
	bool ShouldDrink();
	bool CanDrink();
	std::function<bool(void)> IsDrinkingOrShouldDrinkAndCanDrink();
	Decision* DrinkWater();

	// To find water
	Decision* ToFindWater();
	std::function<bool(void)> ShouldDrinkAndCanNotDrink();
	Decision* FindWater();

	// To hunt
	Decision* ToHunt();
	bool IsHunting();
	bool ShouldHunt();
	bool CanHunt();
	std::function<bool(void)> IsHuntingOrShouldHuntAndCanHunt();
	Decision* Hunt();

	// To find prey
	Decision* ToFindPrey();
	std::function<bool(void)> ShouldHuntAndCanNotHunt();
	Decision* FindPrey();
	Decision* Wander();
};

