#pragma once
#include "DecisionTree.h"
#include "raylib.h"

class ChickenDecisionTree : public DecisionTree
{
public:
	std::vector<Entity>& mEnemies;
	ChickenDecisionTree(
		Entity entity, 
		std::vector<std::vector<Entity>>& grid, 
		float cellWidth, 
		float cellHeight, 
		Systems::AStar* astar,
		std::vector<Entity>& enemies
	);

	void Init();

	// To flee
	Decision* ToFlee();
	std::function<bool(void)> ShouldFlee();
	Decision* Flee();

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

	// To eat
	Decision* ToEat();
	bool IsEating();
	bool ShouldEat();
	bool CanEat();
	std::function<bool(void)> IsEatingOrShouldEatAndCanEat();
	Decision* EatFood();

	// To find food
	Decision* ToFindFood();
	std::function<bool(void)> ShouldEatAndCanNotEat();
	Decision* FindFood();

	// Wander
	Decision* Wander();
};

