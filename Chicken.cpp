#include "Chicken.h"


Chicken::Chicken(std::vector<std::vector<Entity>>& grid, float cellWidth, float cellHeight, Systems::AStar* aStar, std::vector<Entity>& enemies) : mGrid(grid)
{
	mEntity = Registry::GetInstance()->CreateEntity();
	Init();
	mDecisionTree = new ChickenDecisionTree(mEntity, grid, cellWidth, cellHeight, aStar, enemies);
}

Chicken::~Chicken()
{
	Registry* registry = Registry::GetInstance();
	Components::Avoid& avoid = registry->GetComponent<Components::Avoid>(mEntity);
	for (Obstacle* obstacle : avoid.obstacles)
	{
		delete obstacle;
	}
	delete mDecisionTree;
}

void Chicken::Init()
{
	Registry* registry = Registry::GetInstance();
	std::vector<Entity> canadates;

	for (int i = 0; i < mGrid.size(); i++)
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
	}

	int randIndex = rand() % canadates.size();
	Entity canadate = canadates[randIndex];
	Math::Vector3 pos = registry->GetComponent<Components::Transform>(canadate).position;

	registry->AddComponent(
		mEntity,
		Components::Transform{
			pos,
			0,
			1.0f
		}
	);

	registry->AddComponent(
		mEntity,
		Components::RigidBody{
			{ 0.0f, 0.0f, 0.0f },
			{ 0.0f, 0.0f, 0.0f },
			100.0f
		}
	);

	registry->AddComponent(
		mEntity,
		Components::Seek{
			{},
			200.0f
		}
	);

	registry->AddComponent(
		mEntity,
		Components::Avoid{
			{},
			5000.0f,
			30
		}
	);

	registry->AddComponent(
		mEntity,
		Components::Sprites
		{
			{
				Components::Sprite
				{
					TextureManager::GetInstance()->Get("chicken_walk"),
					0, 96,
					32, 32
				},
				Components::Sprite
				{
					TextureManager::GetInstance()->Get("chicken_walk"),
					32, 96,
					32, 32
				},
				Components::Sprite
				{
					TextureManager::GetInstance()->Get("chicken_walk"),
					64, 96,
					32, 32
				},
				Components::Sprite
				{
					TextureManager::GetInstance()->Get("chicken_walk"),
					96, 96,
					32, 32
				},
				Components::Sprite
				{
					TextureManager::GetInstance()->Get("chicken_walk"),
					0, 32,
					32, 32
				},
				Components::Sprite
				{
					TextureManager::GetInstance()->Get("chicken_walk"),
					32, 32,
					32, 32
				},
				Components::Sprite
				{
					TextureManager::GetInstance()->Get("chicken_walk"),
					64, 32,
					32, 32
				},
				Components::Sprite
				{
					TextureManager::GetInstance()->Get("chicken_walk"),
					96, 32,
					32, 32
				},
				Components::Sprite
				{
					TextureManager::GetInstance()->Get("chicken_walk"),
					0, 0,
					32, 32
				},
				Components::Sprite
				{
					TextureManager::GetInstance()->Get("chicken_walk"),
					32, 0,
					32, 32
				},
				Components::Sprite
				{
					TextureManager::GetInstance()->Get("chicken_walk"),
					64, 0,
					32, 32
				},
				Components::Sprite
				{
					TextureManager::GetInstance()->Get("chicken_walk"),
					96, 0,
					32, 32
				},
				Components::Sprite
				{
					TextureManager::GetInstance()->Get("chicken_walk"),
					0, 64,
					32, 32
				},
				Components::Sprite
				{
					TextureManager::GetInstance()->Get("chicken_walk"),
					32, 64,
					32, 32
				},
				Components::Sprite
				{
					TextureManager::GetInstance()->Get("chicken_walk"),
					64, 64,
					32, 32
				},
				Components::Sprite
				{
					TextureManager::GetInstance()->Get("chicken_walk"),
					96, 64,
					32, 32
				},
			Components::Sprite
				{
					TextureManager::GetInstance()->Get("chicken_eat"),
					0, 96,
					32, 32
				},
				Components::Sprite
				{
					TextureManager::GetInstance()->Get("chicken_eat"),
					32, 96,
					32, 32
				},
				Components::Sprite
				{
					TextureManager::GetInstance()->Get("chicken_eat"),
					64, 96,
					32, 32
				},
				Components::Sprite
				{
					TextureManager::GetInstance()->Get("chicken_eat"),
					96, 96,
					32, 32
				},
				Components::Sprite
				{
					TextureManager::GetInstance()->Get("chicken_eat"),
					0, 32,
					32, 32
				},
				Components::Sprite
				{
					TextureManager::GetInstance()->Get("chicken_eat"),
					32, 32,
					32, 32
				},
				Components::Sprite
				{
					TextureManager::GetInstance()->Get("chicken_eat"),
					64, 32,
					32, 32
				},
				Components::Sprite
				{
					TextureManager::GetInstance()->Get("chicken_eat"),
					96, 32,
					32, 32
				},
				Components::Sprite
				{
					TextureManager::GetInstance()->Get("chicken_eat"),
					0, 0,
					32, 32
				},
				Components::Sprite
				{
					TextureManager::GetInstance()->Get("chicken_eat"),
					32, 0,
					32, 32
				},
				Components::Sprite
				{
					TextureManager::GetInstance()->Get("chicken_eat"),
					64, 0,
					32, 32
				},
				Components::Sprite
				{
					TextureManager::GetInstance()->Get("chicken_eat"),
					96, 0,
					32, 32
				},
				Components::Sprite
				{
					TextureManager::GetInstance()->Get("chicken_eat"),
					0, 64,
					32, 32
				},
				Components::Sprite
				{
					TextureManager::GetInstance()->Get("chicken_eat"),
					32, 64,
					32, 32
				},
				Components::Sprite
				{
					TextureManager::GetInstance()->Get("chicken_eat"),
					64, 64,
					32, 32
				},
				Components::Sprite
				{
					TextureManager::GetInstance()->Get("chicken_eat"),
					96, 64,
					32, 32
				}
			}
		}
	);

	registry->AddComponent(
		mEntity,
		Components::Rect
		{
			64, 64, 32, 32
		}
	);

	registry->AddComponent(
		mEntity,
		Components::SpriteAnimation
		{
			{
				{"idleRigit", {0}},
				{"idleLeft", {4}},
				{"idleUp", {8}},
				{"idleDown", {12}},
				{"moveRight", {0, 1, 2, 3}},
				{"moveLeft", {4, 5, 6, 7}},
				{"moveUp", {8, 9, 10, 11}},
				{"moveDown", {12, 13, 14, 15}},
				{"eatRight", {16, 17, 18, 19}},
				{"eatLeft", {20, 21, 22, 23}},
				{"eatUp", {24, 25, 26, 27}},
				{"eatDown", {28, 29, 30, 31}}
			},
			0.2f,
			[registry](Entity mEntity) {
				Components::SpriteAnimation& spriteAnimation = Registry::GetInstance()->GetComponent<Components::SpriteAnimation>(mEntity);
				Components::RigidBody& rigidBody = Registry::GetInstance()->GetComponent<Components::RigidBody>(mEntity);
				Components::AnimalStat& animalStat = registry->GetComponent<Components::AnimalStat>(mEntity);
				std::string previousState = spriteAnimation.state;
				float xDiff = abs(rigidBody.velocity.x);
				float yDiff = abs(rigidBody.velocity.y);

				if (rigidBody.velocity.x == 0 && rigidBody.velocity.y == 0)
				{
					if (spriteAnimation.state == "moveLeft")
					{
						if (animalStat.eating || animalStat.drinking)
						{
							spriteAnimation.state = "eatLeft";
						}
						else
						{
							spriteAnimation.state = "idleLeft";
						}
					}
					else if (spriteAnimation.state == "moveRight")
					{
						if (animalStat.eating || animalStat.drinking)
						{
							spriteAnimation.state = "eatRight";
						}
						else {
							spriteAnimation.state = "idleRight";
						}
					}
					else if (spriteAnimation.state == "moveUp")
					{
						if (animalStat.eating || animalStat.drinking)
						{
							spriteAnimation.state = "eatUp";
						}
						else {
							spriteAnimation.state = "idleUp";
						}
					}
					else if (spriteAnimation.state == "moveDown")
					{
						if (animalStat.eating || animalStat.drinking)
						{
							spriteAnimation.state = "eatDown";
						}
						else
						{
							spriteAnimation.state = "idleDown";
						}
					}
				}
				else {
					if (xDiff >= yDiff)
					{
						if (rigidBody.velocity.x > 0)
						{
							spriteAnimation.state = "moveRight";
						}
						else if (rigidBody.velocity.x < 0)
						{
							spriteAnimation.state = "moveLeft";
						}
					}
					else {
						if (rigidBody.velocity.y > 0)
						{
							spriteAnimation.state = "moveDown";
						}
						else if (rigidBody.velocity.y < 0)
						{
							spriteAnimation.state = "moveUp";
						}
					}
				}

				if (spriteAnimation.state != previousState)
				{
					spriteAnimation.frame = 0;
					spriteAnimation.time = 0.0f;
				}
			},
			"idleRigit"
		}
	);


	registry->AddComponent(
		mEntity,
		Components::AnimalStat
		{
			100.0f,
			100.0f,
			3.0f,
			12.0f
		}
	);

	PopulateObstacles();
}

void Chicken::PopulateObstacles()
{
	Registry* registry = Registry::GetInstance();
	Components::Avoid& avoid = Registry::GetInstance()->GetComponent<Components::Avoid>(mEntity);
	for (int i = 0; i < mGrid.size(); i++) {
		for (int j = 0; j < mGrid[i].size(); j++) {
			Components::Node& aStarNode = registry->GetComponent<Components::Node>(mGrid[i][j]);
			Components::Tile& tile = registry->GetComponent<Components::Tile>(mGrid[i][j]);
			if (aStarNode.blocked)
			{
				Obstacle* obstacle = new Obstacle{ mGrid[i][j], 50 };
				avoid.obstacles.push_back(obstacle);
			}
		}
	}
}


