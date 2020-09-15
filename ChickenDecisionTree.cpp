#include "ChickenDecisionTree.h"

ChickenDecisionTree::ChickenDecisionTree(
	Entity entity,
	std::vector<std::vector<Entity>>& grid, 
	float cellWidth,
	float cellHeight,
	Systems::AStar* astar,
	std::vector<Entity>& enemies) : DecisionTree(entity, grid, cellWidth, cellHeight, astar), mEnemies(enemies)
{
	Init();
}


void ChickenDecisionTree::Init()
{
	mRoot = ToFlee();
	Registry::GetInstance()->AddComponent(
		mEntity,
		Components::DecisionTree{
			mRoot,
			0.1f,
			GetRandomValue(0, 10) / 10.0f
		}
	);
}

Decision* ChickenDecisionTree::ToFlee()
{
	Decision* decision = new Decision{};
	decision->mDecision = MakeDecision(decision, []() {});
	decision->mCondition = ShouldFlee();
	decision->mTrueBranch = Flee();
	decision->mFalseBranch = ToDrink();
	return decision;
}

std::function<bool(void)> ChickenDecisionTree::ShouldFlee()
{
	return [this]() {
		Registry* registry = Registry::GetInstance();
		Components::Transform& transform = registry->GetComponent<Components::Transform>(mEntity);
		if (mEnemies.size() == 0) return false;
		Components::Transform& enemyTransform = registry->GetComponent<Components::Transform>(mEnemies[0]);
		return Math::Magnitude(transform.position - enemyTransform.position) < 150;
	};
}

Decision* ChickenDecisionTree::Flee()
{
	return new Decision{
		[this]() {
				Registry* registry = Registry::GetInstance();
				Components::Seek& seek = registry->GetComponent<Components::Seek>(mEntity);
				Components::Transform& transform = registry->GetComponent<Components::Transform>(mEntity);
				Components::AnimalStat& stats = registry->GetComponent<Components::AnimalStat>(mEntity);
				stats.decision = "Flee";

				stats.drinking = false;
				stats.eating = false;
				int nearestRow = transform.position.y / mCellHeight;
				int nearestCol = transform.position.x / mCellWidth;

				int minRow = fmax(nearestRow - 3, 0);
				int minCol = fmax(nearestCol - 3, 0);
				int maxRow = fmin(nearestRow + 3, mGrid.size() - 1);
				int maxCol = fmin(nearestCol + 3, mGrid[0].size() - 1);

				std::vector<Entity> canadates;

				for (int i = minRow; i <= maxRow; i++)
				{
					for (int j = minCol; j <= maxCol; j++)
					{
						Components::Tile& tile = registry->GetComponent<Components::Tile>(mGrid[i][j]);
						if (tile.type == TILETYPE::EMPTY)
						{
							if (nearestRow != i || nearestCol != j)
							{	
								canadates.push_back(mGrid[i][j]);
							}
						}
					}
				}

				float furthest = 0;
				int longestSteps = 0;
				Entity cell = 0;


				for (int i = 0; i < canadates.size(); i++)
				{
					ResetAStarGrid();
					Components::Transform& enemyTransform = registry->GetComponent<Components::Transform>(mEnemies[0]);
					int nearestEnemyRow = enemyTransform.position.y / mCellHeight;
					int nearestEnemyCol = enemyTransform.position.x / mCellWidth;
					
					std::vector<Entity> path = mAStar->Search(mGrid[nearestEnemyRow][nearestEnemyCol], canadates[i]);

					if (path.size() >= longestSteps)
					{
						float dist = Math::Magnitude(enemyTransform.position - transform.position);
						if (dist > furthest)
						{
							furthest = dist;
						}
						cell = canadates[i];
						longestSteps = path.size();
					}
				}

				seek.target = mAStar->Search(mGrid[nearestRow][nearestCol], cell);
			}
		
	};
}




Decision* ChickenDecisionTree::ToDrink()
{
	Decision* decision = new Decision{};
	decision->mDecision = MakeDecision(decision, []() {});
	decision->mCondition = IsDrinkingOrShouldDrinkAndCanDrink();
	decision->mTrueBranch = DrinkWater();
	decision->mFalseBranch = ToFindWater();
	return decision;
}


bool ChickenDecisionTree::IsDrinking()
{
	Components::AnimalStat& animalStat = Registry::GetInstance()->GetComponent<Components::AnimalStat>(mEntity);
	return animalStat.drinking && animalStat.thirst < 100;
}
bool ChickenDecisionTree::ShouldDrink()
{
	Registry* registry = Registry::GetInstance();
	Components::AnimalStat& animalStat = registry->GetComponent<Components::AnimalStat>(mEntity);
	return animalStat.thirst < 70 && animalStat.thirst <= animalStat.hunger;
}
bool ChickenDecisionTree::CanDrink()
{
	Registry* registry = Registry::GetInstance();
	Components::AnimalStat& animalStat = registry->GetComponent<Components::AnimalStat>(mEntity);
	Components::Transform& transform = registry->GetComponent<Components::Transform>(mEntity);

	float minDist = FLT_MAX;
	for (int i = 0; i < mGrid.size(); i++)
	{
		for (int j = 0; j < mGrid[i].size(); j++)
		{
			Entity cell = mGrid[i][j];
			Components::Tile& tile = registry->GetComponent<Components::Tile>(cell);
			Components::Transform& cellTransform = registry->GetComponent<Components::Transform>(cell);
			if (tile.type == TILETYPE::WATER)
			{
				float dist = Math::Magnitude(transform.position - cellTransform.position);
				if (dist < minDist)
				{
					minDist = dist;
				}
			}
		}
	}
	return minDist <= 60.0f;
}
std::function<bool(void)> ChickenDecisionTree::IsDrinkingOrShouldDrinkAndCanDrink()
{
	return [this]() {
		return IsDrinking() || (ShouldDrink() && CanDrink());
	};
}

Decision* ChickenDecisionTree::DrinkWater()
{
	return new Decision{
		[this]() {
			Components::AnimalStat& animalStat = Registry::GetInstance()->GetComponent<Components::AnimalStat>(mEntity);
			animalStat.decision = "DrinkWater";
			animalStat.drinking = true;
		}
	};
}

Decision* ChickenDecisionTree::ToFindWater()
{
	Decision* decision = new Decision{};
	decision->mDecision = MakeDecision(
		decision,
		[this]() {
			Components::AnimalStat& animalStat = Registry::GetInstance()->GetComponent<Components::AnimalStat>(mEntity);
			animalStat.drinking = false;
		}
	);
	decision->mCondition = ShouldDrinkAndCanNotDrink();
	decision->mTrueBranch = FindWater();
	decision->mFalseBranch = ToEat();
	return decision;
}

std::function<bool(void)> ChickenDecisionTree::ShouldDrinkAndCanNotDrink()
{
	return [this]() {
		return ShouldDrink() && !CanDrink();
	};
}

Decision* ChickenDecisionTree::FindWater()
{
	return new Decision{
		[this]() {
			Registry* registry = Registry::GetInstance();

			Components::AnimalStat& stats = registry->GetComponent<Components::AnimalStat>(mEntity);
			stats.decision = "FindWater";
			Components::Seek& seek = registry->GetComponent<Components::Seek>(mEntity);
			Components::Transform& transform = registry->GetComponent<Components::Transform>(mEntity);
			if (seek.target.size() == 0) {
				int nearestRow = transform.position.y / mCellHeight;
				int nearestCol = transform.position.x / mCellWidth;
				std::vector<Entity> canadates;
				for (int i = 0; i < mGrid.size(); i++)
				{
					for (int j = 0; j < mGrid[i].size(); j++)
					{
						Entity cell = mGrid[i][j];
						Components::Tile& tile = registry->GetComponent<Components::Tile>(cell);
						Components::Transform& cellTransform = registry->GetComponent<Components::Transform>(cell);
						if (tile.type == TILETYPE::WATER)
						{
							canadates.push_back(cell);
						}
					}
				}

				unsigned int minSteps = UINT16_MAX;
				std::vector<Entity> shortestPath;

				for (Entity canadate : canadates)
				{
					Components::Transform& enemyTransform = registry->GetComponent<Components::Transform>(mEnemies[0]);
					Components::Transform& canadateTransform = registry->GetComponent<Components::Transform>(canadate);

					if (Math::Magnitude(canadateTransform.position - enemyTransform.position) > 500)
					{
						ResetAStarGrid();
						std::vector<Entity> path = mAStar->Search(mGrid[nearestRow][nearestCol], canadate);
						if (path.size() < minSteps)
						{
							minSteps = path.size();
							shortestPath = path;
						}
					}
				}
				seek.target = shortestPath;
			}
		}
	};
}

Decision* ChickenDecisionTree::ToEat()
{
	Decision* decision = new Decision{};
	decision->mDecision = MakeDecision(decision, []() {});
	decision->mCondition = IsEatingOrShouldEatAndCanEat();
	decision->mTrueBranch = EatFood();
	decision->mFalseBranch = ToFindFood();
	return decision;
}

bool ChickenDecisionTree::IsEating()
{
	Components::AnimalStat& animalStat = Registry::GetInstance()->GetComponent<Components::AnimalStat>(mEntity);
	return animalStat.eating && animalStat.hunger < 100;
}

bool ChickenDecisionTree::ShouldEat()
{
	Components::AnimalStat& animalStat = Registry::GetInstance()->GetComponent<Components::AnimalStat>(mEntity);
	return animalStat.hunger < 70;
}

bool ChickenDecisionTree::CanEat()
{
	Registry* registry = Registry::GetInstance();
	Components::AnimalStat& animalStat = registry->GetComponent<Components::AnimalStat>(mEntity);
	Components::Transform& transform = registry->GetComponent<Components::Transform>(mEntity);

	float minDist = FLT_MAX;
	for (int i = 0; i < mGrid.size(); i++)
	{
		for (int j = 0; j < mGrid[i].size(); j++)
		{
			Entity cell = mGrid[i][j];
			Components::Tile& tile = registry->GetComponent<Components::Tile>(cell);
			Components::Transform& cellTransform = registry->GetComponent<Components::Transform>(cell);
			if (tile.type == TILETYPE::FOOD)
			{
				float dist = Math::Magnitude(transform.position - cellTransform.position);
				if (dist < minDist)
				{
					minDist = dist;
				}
			}
		}
	}
	return minDist <= 60.0f;
}

std::function<bool(void)> ChickenDecisionTree::IsEatingOrShouldEatAndCanEat()
{
	return [this]() {
		return IsEating() || (ShouldEat() && CanEat());
	};
}

Decision* ChickenDecisionTree::EatFood()
{
	return new Decision
	{
		[this]()
		{
			Components::AnimalStat& animalStat = Registry::GetInstance()->GetComponent<Components::AnimalStat>(mEntity);
			animalStat.decision = "EatFood";
			animalStat.eating = true;
		}
	};
}

Decision* ChickenDecisionTree::ToFindFood()
{
	Decision* decision = new Decision{};
	decision->mDecision = MakeDecision(
		decision,
		[this]() {
			Components::AnimalStat& animalStat = Registry::GetInstance()->GetComponent<Components::AnimalStat>(mEntity);
			animalStat.eating = false;
		}
	);
	decision->mCondition = ShouldEatAndCanNotEat();
	decision->mTrueBranch = FindFood();
	decision->mFalseBranch = Wander();
	return decision;
}

std::function<bool(void)> ChickenDecisionTree::ShouldEatAndCanNotEat()
{
	return [this]() {
		return ShouldEat() && !CanEat();
	};
}

Decision* ChickenDecisionTree::FindFood()
{
	return new Decision{
		[this]() {
			Registry* registry = Registry::GetInstance();
			Components::Seek& seek = registry->GetComponent<Components::Seek>(mEntity);
			Components::Transform& transform = registry->GetComponent<Components::Transform>(mEntity);

			Components::AnimalStat& stats = registry->GetComponent<Components::AnimalStat>(mEntity);
			stats.decision = "FindFood";
			if (seek.target.size() == 0) {
				int nearestRow = transform.position.y / mCellHeight;
				int nearestCol = transform.position.x / mCellWidth;
				std::vector<Entity> canadates;
				for (int i = 0; i < mGrid.size(); i++)
				{
					for (int j = 0; j < mGrid[i].size(); j++)
					{
						Entity cell = mGrid[i][j];
						Components::Tile& tile = registry->GetComponent<Components::Tile>(cell);
						Components::Transform& cellTransform = registry->GetComponent<Components::Transform>(cell);
						if (tile.type == TILETYPE::FOOD)
						{
							canadates.push_back(cell);
						}
					}
				}

				unsigned int minSteps = UINT16_MAX;
				std::vector<Entity> shortestPath;

				for (Entity canadate : canadates)
				{
					Components::Transform& enemyTransform = registry->GetComponent<Components::Transform>(mEnemies[0]);
					Components::Transform& canadateTransform = registry->GetComponent<Components::Transform>(canadate);

					if (Math::Magnitude(canadateTransform.position - enemyTransform.position) > 500)
					{
						ResetAStarGrid();
						std::vector<Entity> path = mAStar->Search(mGrid[nearestRow][nearestCol], canadate);
						if (path.size() < minSteps)
						{
							minSteps = path.size();
							shortestPath = path;
						}
					}
				}
				seek.target = shortestPath;
			}
		}
	};
}

Decision* ChickenDecisionTree::Wander()
{
	return new Decision
	{
		[this]() {
			Registry* registry = Registry::GetInstance();
			Components::Seek& seek = registry->GetComponent<Components::Seek>(mEntity);
			Components::Transform& transform = registry->GetComponent<Components::Transform>(mEntity);

			Components::AnimalStat& stats = registry->GetComponent<Components::AnimalStat>(mEntity);
			stats.decision = "Wander";

			if (seek.target.size() == 0) {
				int nearestRow = transform.position.y / mCellHeight;
				int nearestCol = transform.position.x / mCellWidth;
				int minRow = fmax(nearestRow - 5, 0);
				int minCol = fmax(nearestCol - 5, 0);
				int maxRow = fmin(nearestRow + 5, mGrid.size() - 1);
				int maxCol = fmin(nearestCol + 5, mGrid[0].size() - 1);

				std::vector<Entity> canadates;

				for (int i = minRow; i <= maxRow; i++)
				{
					for (int j = minCol; j <= maxCol; j++)
					{
						Components::Tile& tile = registry->GetComponent<Components::Tile>(mGrid[i][j]);
						if (tile.type == TILETYPE::EMPTY)
						{
							if (nearestRow != i || nearestCol != j)
							{
								canadates.push_back(mGrid[i][j]);
							}
						}
					}
				}


				int randIndex = rand() % canadates.size();
				ResetAStarGrid();
				std::vector<Entity> path = mAStar->Search(mGrid[nearestRow][nearestCol], canadates[randIndex]);
				seek.target = path;
			}
		}
	};
}
