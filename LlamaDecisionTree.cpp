#include "LlamaDecisionTree.h"

LlamaDecisionTree::LlamaDecisionTree(
	Entity entity,
	std::vector<std::vector<Entity>>& grid,
	float cellWidth,
	float cellHeight,
	Systems::AStar* astar,
	std::vector<Entity>& preys
) : DecisionTree(entity, grid, cellWidth, cellHeight, astar), mPreys(preys)
{
	Init();
}


void LlamaDecisionTree::Init()
{
	mRoot = ToDrink();
	Registry::GetInstance()->AddComponent(
		mEntity,
		Components::DecisionTree{
			mRoot,
			0.1f,
			GetRandomValue(0, 10) / 10.0f
	});
}

Decision* LlamaDecisionTree::ToDrink()
{
	Decision* decision = new Decision{};
	decision->mDecision = MakeDecision(decision, []() {});
	decision->mCondition = IsDrinkingOrShouldDrinkAndCanDrink();
	decision->mTrueBranch = DrinkWater();
	decision->mFalseBranch = ToFindWater();
	return decision;
}
bool LlamaDecisionTree::IsDrinking()
{
	Components::AnimalStat& animalStat = Registry::GetInstance()->GetComponent<Components::AnimalStat>(mEntity);
	return animalStat.drinking && animalStat.thirst < 100;
}
bool LlamaDecisionTree::ShouldDrink()
{
	Registry* registry = Registry::GetInstance();
	Components::AnimalStat& animalStat = registry->GetComponent<Components::AnimalStat>(mEntity);
	return animalStat.thirst < 70 && animalStat.thirst <= animalStat.hunger;
}
bool LlamaDecisionTree::CanDrink()
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
std::function<bool(void)> LlamaDecisionTree::IsDrinkingOrShouldDrinkAndCanDrink()
{
	return [this]() {
		return IsDrinking() || (ShouldDrink() && CanDrink());
	};
}
Decision* LlamaDecisionTree::DrinkWater()
{
	return new Decision{
		[this]() {
			Registry* registry = Registry::GetInstance();
			Components::AnimalStat& stats = registry->GetComponent<Components::AnimalStat>(mEntity);
			stats.decision = "DrinkWater";
			Components::AnimalStat& animalStat = registry->GetComponent<Components::AnimalStat>(mEntity);
			animalStat.drinking = true;
		}
	};
}
Decision* LlamaDecisionTree::ToFindWater()
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
	decision->mFalseBranch = ToHunt();
	return decision;
}
std::function<bool(void)> LlamaDecisionTree::ShouldDrinkAndCanNotDrink()
{
	return [this]() {
		return ShouldDrink() && !CanDrink();
	};
}
Decision* LlamaDecisionTree::FindWater()
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
					ResetAStarGrid();
					std::vector<Entity> path = mAStar->Search(mGrid[nearestRow][nearestCol], canadate);
					if (path.size() < minSteps)
					{
						minSteps = path.size();
						shortestPath = path;
					}
				}
				seek.target = shortestPath;
			}
		}
	};
}

Decision* LlamaDecisionTree::ToHunt()
{
	Decision* decision = new Decision{};
	decision->mDecision = MakeDecision(decision, []() {});
	decision->mCondition = IsHuntingOrShouldHuntAndCanHunt();
	decision->mTrueBranch = Hunt();
	decision->mFalseBranch = ToFindPrey();
	return decision;
}

bool LlamaDecisionTree::IsHunting()
{
	Components::AnimalStat& animalStat = Registry::GetInstance()->GetComponent<Components::AnimalStat>(mEntity);
	return animalStat.hunting && animalStat.hunger < 100;
}

bool LlamaDecisionTree::ShouldHunt()
{
	Components::AnimalStat& animalStat = Registry::GetInstance()->GetComponent<Components::AnimalStat>(mEntity);
	return animalStat.hunger < 70;
}

bool LlamaDecisionTree::CanHunt()
{
	Registry* registry = Registry::GetInstance();
	Components::AnimalStat& animalStat = registry->GetComponent<Components::AnimalStat>(mEntity);
	Components::Transform& transform = registry->GetComponent<Components::Transform>(mEntity);

	float minDist = FLT_MAX;

	for (Entity prey : mPreys)
	{
		Components::Transform& preyTransform = registry->GetComponent<Components::Transform>(prey);
		float dist = Math::Magnitude(transform.position - preyTransform.position);
		if (dist < minDist)
		{
			minDist = dist;
		}
	}
	return minDist <= 100.0f;
}

std::function<bool(void)> LlamaDecisionTree::IsHuntingOrShouldHuntAndCanHunt()
{
	return [this]() {
		return (ShouldHunt() && CanHunt());
	};
}

Decision* LlamaDecisionTree::Hunt()
{
	return new Decision{
		[this]() {
			Registry* registry = Registry::GetInstance();
			Components::AnimalStat& stats = registry->GetComponent<Components::AnimalStat>(mEntity);
			stats.decision = "Hunt";
			Components::AnimalStat& animalStat = registry->GetComponent<Components::AnimalStat>(mEntity);
			animalStat.hunger += 20;
			if (animalStat.hunger >= 100)
			{
				animalStat.hunger = 100;
			}
		}
	};
}

Decision* LlamaDecisionTree::ToFindPrey()
{
	Decision* decision = new Decision{};
	decision->mDecision = MakeDecision(
		decision,
		[this]() {
			Components::AnimalStat& animalStat = Registry::GetInstance()->GetComponent<Components::AnimalStat>(mEntity);
			//animalStat.eating = false;
		}
	);
	decision->mCondition = ShouldHuntAndCanNotHunt();
	decision->mTrueBranch = FindPrey();
	decision->mFalseBranch = Wander();
	return decision;
}

std::function<bool(void)> LlamaDecisionTree::ShouldHuntAndCanNotHunt()
{
	return [this]() {
		return ShouldHunt() && !CanHunt();
	};
}

Decision* LlamaDecisionTree::FindPrey()
{
	return new Decision{
		[this]() {
			Registry* registry = Registry::GetInstance();
			Components::AnimalStat& stats = registry->GetComponent<Components::AnimalStat>(mEntity);
			stats.decision = "FindPrey";
			Components::Seek& seek = registry->GetComponent<Components::Seek>(mEntity);
			Components::Transform& transform = registry->GetComponent<Components::Transform>(mEntity);
			
			int nearestRow = transform.position.y / mCellHeight;
			int nearestCol = transform.position.x / mCellWidth;

			unsigned int minSteps = UINT16_MAX;
			std::vector<Entity> shortestPath;

			for (Entity prey : mPreys)
			{
				ResetAStarGrid();
				Components::Transform& preyTransform = registry->GetComponent<Components::Transform>(prey);

				int nearestPreyRow = preyTransform.position.y / mCellHeight;
				int nearestPreyCol = preyTransform.position.x / mCellWidth;

				std::vector<Entity> path = mAStar->Search(mGrid[nearestRow][nearestCol], mGrid[nearestPreyRow][nearestPreyCol]);
				if (path.size() < minSteps)
				{
					minSteps = path.size();
					shortestPath = path;
				}
			}
			seek.target = shortestPath;
		}
	};
}

Decision* LlamaDecisionTree::Wander()
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
				//std::vector<Entity> canadates;
		/*		for (int i = 0; i < mGrid.size(); i++)
				{
					for (int j = 0; j < mGrid[i].size(); j++)
					{
						Entity cell = mGrid[i][j];
						Components::Tile& tile = registry->GetComponent<Components::Tile>(cell);
						Components::Node& node = registry->GetComponent<Components::Node>(cell);
						if (tile.type == TILETYPE::EMPTY)
						{
							canadates.push_back(cell);
						}
					}
				}*/

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

				int randIndex = rand() % canadates.size();
				ResetAStarGrid();
				std::vector<Entity> path = mAStar->Search(mGrid[nearestRow][nearestCol], canadates[randIndex]);
				seek.target = path;
			}
		}
	};
}
