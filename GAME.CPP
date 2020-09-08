#include "Game.h"
#include <iostream>
#include <time.h>
#include <stdlib.h>
void Game::Init()
{
	srand(time(nullptr));
	Registry::Create();
	mRegistry = Registry::GetInstance();
	RegisterComponents();
	RegisterSystems();

	CreateTextureManager();
	mRows = 18;
	mCols = 18;
	mCellWidth = 64;
	mCellHeight = 64;
	mGrid = std::vector<std::vector<Entity>>(mRows,	std::vector<Entity>(mCols, 0));

	for (int i = 0; i < mRows; i++)
	{
		for (int j = 0; j < mCols; j++)
		{
			mGrid[i][j] = CreateGridCell(i, j);
		}
	}

	LoadTileMap();

	for (int i = 0; i < 5; i++)
	{
		Entity chicken = CreateChicken();
		mChickens.push_back(chicken);
	}

	for (Entity chicken : mChickens)
	{
		PopulateObstacles(chicken);
		mDecisions.push_back(CreateDecisionTree(chicken));
	}

	mTileMap->Init();

	mDeltaTime = 0.0f;
} 

void Game::Update()
{
	mDeltaTime = GetFrameTime();
	mAnimal->Update(mDeltaTime);
	mMakeDecision->Update(mDeltaTime);
	mSeekBehavior->Update(mDeltaTime);
	mAvoidBehavior->Update(mDeltaTime);
	mPhysics->Update(mDeltaTime);
	mSpriteAnimator->Update(mDeltaTime);
}

void Game::Draw()
{
	mSpriteRenderer->Draw();
	//mAStar->Debug();
	mAnimal->Debug();
	mSeekBehavior->Debug();
	mAvoidBehavior->Debug();
}

void Game::Shutdown()
{

	for (Decision* d : mDecisions)
	{
		d->DeleteChildren();
	}

	for (std::vector<Obstacle*> obstacles : mObstacles)
	{
		for (Obstacle* obstacle : obstacles)
		{
			delete obstacle;
		}
	}
	//mDecisionRoot->DeleteChildren();
	Registry::Destroy();
	TextureManager::Destroy();
}
void Game::RegisterComponents()
{
	mRegistry->RegisterComponent<Components::Rect>();
	mRegistry->RegisterComponent<Components::Transform>();
	mRegistry->RegisterComponent<Components::Sprites>();
	mRegistry->RegisterComponent<Components::RigidBody>();
	mRegistry->RegisterComponent<Components::SpriteAnimation>();
	mRegistry->RegisterComponent<Components::DecisionTree>();
	mRegistry->RegisterComponent<Components::Tile>();
	mRegistry->RegisterComponent<Components::Node>();
	mRegistry->RegisterComponent<Components::Seek>();
	mRegistry->RegisterComponent<Components::State>();
	mRegistry->RegisterComponent<Components::AnimalStat>();
	mRegistry->RegisterComponent<Components::Avoid>();
}
void Game::RegisterSystems()
{
	mSpriteRenderer = mRegistry->RegisterSystem<Systems::SpriteRenderer>();
	mRegistry->SetSystemSignatureV<Systems::SpriteRenderer>({
		mRegistry->GetComponentType<Components::Sprites>(),
		mRegistry->GetComponentType<Components::Rect>(),
		mRegistry->GetComponentType<Components::Transform>()
	});

	mPhysics = mRegistry->RegisterSystem<Systems::Physics>();
	mRegistry->SetSystemSignatureV<Systems::Physics>({
		mRegistry->GetComponentType<Components::Transform>(),
		mRegistry->GetComponentType<Components::RigidBody>()
	});

	mSpriteAnimator = mRegistry->RegisterSystem<Systems::SpriteAnimator>();
	mRegistry->SetSystemSignatureV<Systems::SpriteAnimator>({
		mRegistry->GetComponentType<Components::Sprites>(),
		mRegistry->GetComponentType<Components::SpriteAnimation>()
	});

	mTileMap = mRegistry->RegisterSystem<Systems::TileMap>();
	mRegistry->SetSystemSignatureV<Systems::TileMap>({
		mRegistry->GetComponentType<Components::Sprites>(),
		mRegistry->GetComponentType<Components::Tile>()
	});

	mAStar = mRegistry->RegisterSystem<Systems::AStar>();
	mRegistry->SetSystemSignatureV<Systems::AStar>({
		mRegistry->GetComponentType<Components::Transform>(),
		mRegistry->GetComponentType<Components::Node>()
	});

	mSeekBehavior = mRegistry->RegisterSystem<Systems::SeekBehavior>();
	mRegistry->SetSystemSignatureV<Systems::SeekBehavior>({
		mRegistry->GetComponentType<Components::Seek>(),
		mRegistry->GetComponentType<Components::RigidBody>(),
		mRegistry->GetComponentType<Components::Transform>()
	});

	mAvoidBehavior = mRegistry->RegisterSystem<Systems::AvoidBehavior>();
	mRegistry->SetSystemSignatureV<Systems::AvoidBehavior>({
		mRegistry->GetComponentType<Components::Avoid>(),
		mRegistry->GetComponentType<Components::RigidBody>(),
		mRegistry->GetComponentType<Components::Transform>()
	});

	mMakeDecision = mRegistry->RegisterSystem<Systems::MakeDecision>();
	mRegistry->SetSystemSignatureV<Systems::MakeDecision>({
		mRegistry->GetComponentType<Components::DecisionTree>()
	});

	mAnimal = mRegistry->RegisterSystem<Systems::Animal>();
	mRegistry->SetSystemSignatureV<Systems::Animal>({
		mRegistry->GetComponentType<Components::AnimalStat>()
	});
}
void Game::CreateTextureManager()
{
	TextureManager::Create();
	TextureManager* tm = TextureManager::GetInstance();
	int indices[] = {
		  0,   1,   2,   3,  4, 5,  6, 7,   8,  9, 10, 11, 12, 13, 14, 15,
		 16,   1,  18,   3, 20, 5, 22, 7,   8,  9, 10, 11, 12, 13, 14, 15,
		 32,   1,   2,   3, 36, 5,  6, 7,  40,  9, 10, 11, 44, 13, 14, 15,
		 48,   1,  18,   3, 52, 5, 22, 7,  40,  9, 10, 11, 44, 13, 14, 15,
		 64,  65,   2,   3,  4, 5,  6, 7,  72, 73, 10, 11, 12, 13, 14, 15,
		 80,  65,  18,   3, 20, 5, 22, 7,  72, 73, 10, 11, 12, 13, 14, 15,
		 96,  65,   2,   3, 36, 5,  6, 7, 104, 73, 10, 11, 44, 13, 14, 15,
		112,  65,  18,   3, 52, 5, 22, 7, 104, 73, 10, 11, 44, 13, 14, 15,
		128, 129, 130, 131,  4, 5,  6, 7,   8,  9, 10, 11, 12, 13, 14, 15,
		144, 129, 146, 131, 20, 5, 22, 7,   8,  9, 10, 11, 12, 13, 14, 15,
		160, 129, 130, 131, 36, 5,  6, 7,  40,  9, 10, 11, 44, 13, 14, 15,
		176, 129, 146, 131, 52, 5, 22, 7,  40,  9, 10, 11, 44, 13, 14, 15,
		192, 193, 130, 131,  4, 5,  6, 7,  72, 73, 10, 11, 12, 13, 14, 15,
		208, 193, 146, 131, 20, 5, 22, 7,  72, 73, 10, 11, 12, 13, 14, 15,
		224, 193, 130, 131, 36, 5,  6, 7, 104, 73, 10, 11, 44, 13, 14, 15,
		240, 193, 146, 131, 52, 5, 22, 7, 104, 73, 10, 11, 44, 13, 14, 15
	};
	for (int i = 0; i < 256; i++) {
		std::stringstream key;
		key << "wall_" << i;
		std::stringstream mapKey;
		mapKey << "wall_" << indices[i];
		Texture* t = tm->Get(mapKey.str());
		if (t) {
			tm->Set(key.str(), t);
		}
		else {
			std::stringstream path;
			path << "resources/wall_" << indices[i] << ".png";
			tm->LoadTexture(key.str(), path.str());
		}
	}

	tm->LoadTexture("chicken_walk", "resources/chicken_walk.png");
	tm->LoadTexture("pixel_pack", "resources/pixel_pack.png");
}
Entity Game::CreateGridCell(int i, int j)
{
	Entity entity = mRegistry->CreateEntity();

	mRegistry->AddComponent(
		entity,
		Components::Transform
		{
			Math::Vector3(
				j * mCellWidth + mCellWidth / 2,
				i * mCellHeight + mCellHeight / 2,
				1.0f
			),
			0,
			1.0f
		}
	);

	mRegistry->AddComponent(
		entity,
		Components::Rect
		{
			mCellWidth,
			mCellHeight,
			mCellWidth / 2,
			mCellHeight / 2
		}
	);

	mRegistry->AddComponent(
		entity,
		Components::Sprites
		{
			{
				Components::Sprite
				{
					nullptr,
					0.0f, 0.0f,
					64.0f, 64.0f
				}
			}
		}
	);

	mRegistry->AddComponent(
		entity,
		Components::Tile
		{
			i, j, TILETYPE::EMPTY, 8, 0
		}
	);

	mRegistry->AddComponent(
		entity,
		Components::Node {}
	);
	return entity;
}
Entity Game::CreateChicken()
{
	Entity entity = mRegistry->CreateEntity();

	std::vector<Entity> canadates;

	for (int i = 0; i < mRows; i++)
	{
		for (int j = 0; j < mCols; j++)
		{
			Entity cell = mGrid[i][j];
			Components::Tile& tile = mRegistry->GetComponent<Components::Tile>(cell);
			Components::Node& node = mRegistry->GetComponent<Components::Node>(cell);
			if (tile.type == TILETYPE::EMPTY)
			{
				canadates.push_back(cell);
			}
		}
	}

	int randIndex = rand() % canadates.size();
	Entity canadate = canadates[randIndex];
	Math::Vector3 pos = mRegistry->GetComponent<Components::Transform>(canadate).position;

	mRegistry->AddComponent(
		entity,
		Components::Transform{
			pos,
			0,
			1.0f
		}
	);

	mRegistry->AddComponent(
		entity,
		Components::RigidBody{
			{ 0.0f, 0.0f, 0.0f },
			{ 0.0f, 0.0f, 0.0f },
			200.0f
		}
	);

	mRegistry->AddComponent(
		entity,
		Components::Seek{
			{},
			200.0f
		}
	);

	mRegistry->AddComponent(
		entity,
		Components::Avoid {
			{},
			1000.0f,
			60
		}
	);

	mRegistry->AddComponent(
		entity,
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
				}
			}
		}
	);

	mRegistry->AddComponent(
		entity,
		Components::Rect
		{
			64, 64, 32, 32
		}
	);

	mRegistry->AddComponent(
		entity,
		Components::SpriteAnimation
		{
			{
				{"idleRigit", {0}},
				{"idleLeft", {4}},
				{"moveRight", {0, 1, 2, 3}},
				{"moveLeft", {4, 5, 6, 7}}
			},
			0.2f,
			[this](Entity entity) {
				Components::SpriteAnimation& spriteAnimation = Registry::GetInstance()->GetComponent<Components::SpriteAnimation>(entity);
				Components::RigidBody& rigidBody = Registry::GetInstance()->GetComponent<Components::RigidBody>(entity);
				std::string previousState = spriteAnimation.state;
				if (rigidBody.velocity.x > 0)
				{
					spriteAnimation.state = "moveRight";
				}
				else if (rigidBody.velocity.x < 0)
				{
					spriteAnimation.state = "moveLeft";
				}
				else if (rigidBody.velocity.x == 0)
				{
					if (spriteAnimation.state == "moveLeft")
					{
						spriteAnimation.state = "idleLeft";
					}
					else
					{
						spriteAnimation.state = "idleRight";
					}
				}
				//spriteAnimation.state = currentState;
				if (spriteAnimation.state != previousState)
				{
					spriteAnimation.frame = 0;
					spriteAnimation.time = 0.0f;
				}
			},
			"idleRigit"
		}
	);

	mRegistry->AddComponent(
		entity,
		Components::State {}
	);

	mRegistry->AddComponent(
		entity,
		Components::AnimalStat
		{
			100.0f,
			100.0f,
			3.0f,
			12.0f
		}
	);

	return entity;
}
void Game::LoadTileMap()
{
	std::fstream file;
	file.open("map.dat", std::ios_base::in | std::ios_base::binary);
	if (file.is_open())
	{
		int rows = 0;
		int cols = 0;
		file.read((char*)&rows, sizeof(rows));
		file.read((char*)&cols, sizeof(cols));
		while (!file.eof() && file.peek() != EOF)
		{
			for (int i = 0; i < rows; i++) {
				for (int j = 0; j < cols; j++) {
					Components::Tile& tile = mRegistry->GetComponent<Components::Tile>(mGrid[i][j]);
					Components::Node& aStarNode = mRegistry->GetComponent<Components::Node>(mGrid[i][j]);
					int type;
					file.read((char*)&type, sizeof(type));
					tile.type = TILETYPE(type);
					if (tile.type == TILETYPE::WALL)
					{
						aStarNode.blocked = true;
					}
				}
			}
		}
		file.close();
	}

	int dirX[] = { 0, 1, 0, -1, -1, 1, 1, -1 };
	int dirY[] = { -1, 0, 1, 0, -1, -1, 1, 1 };
	int opp[] = { 2, 3, 0, 1, 6, 7, 4, 5 };

	for (int i = 0; i < mRows; i++) 
	{
		for (int j = 0; j < mCols; j++)
		{
			Entity entity = mGrid[i][j];
			Components::Tile& tile = mRegistry->GetComponent<Components::Tile>(entity);
			Components::Node& aStarNode = mRegistry->GetComponent<Components::Node>(entity);

			for (int k = 0; k < tile.sides; k++)
			{
				unsigned int newI = i + dirY[k];
				unsigned int newJ = j + dirX[k];
				if (newI < 0 || newI > mRows - 1 || newJ < 0 || newJ > mCols - 1) continue;
				Entity other = mGrid[newI][newJ];
				Components::Tile& otherTile = mRegistry->GetComponent<Components::Tile>(other);
				Components::Node& otherAStarNode = mRegistry->GetComponent<Components::Node>(other);

				if (!aStarNode.blocked && !otherAStarNode.blocked) {
					aStarNode.edges.push_back(other);
				}

				if (tile.type == otherTile.type)
				{
					tile.mask.set(k, 0);
				}
				else {
					tile.mask.set(k, 1);
				}
			}
		}
	}
}
void Game::PopulateObstacles(Entity entity)
{
	Components::Avoid& avoid = mRegistry->GetComponent<Components::Avoid>(entity);
	std::vector<Obstacle*> obstacles;
	for (int i = 0; i < mRows; i++) {
		for (int j = 0; j < mCols; j++) {
			Components::Node& aStarNode = mRegistry->GetComponent<Components::Node>(mGrid[i][j]);
			if (aStarNode.blocked)
			{
				Obstacle* obstacle = new Obstacle{ mGrid[i][j], 55 };
				avoid.obstacles.push_back(obstacle);
				obstacles.push_back(obstacle);
			}
		}
	}

	/*for (int i = 0; i < mChickens.size(); i++)
	{
		if (mChickens[i] != entity)
		{
			Obstacle* obstacle = new Obstacle{ mChickens[i], 30 };
			avoid.obstacles.push_back(obstacle);
			obstacles.push_back(obstacle);
		}
	}*/
	mObstacles.push_back(obstacles);
}

Decision* Game::CreateDecisionTree(Entity entity)
{
	//Components::AnimalStat& animalStat = mRegistry->GetComponent<Components::AnimalStat>(entity);
	//Components::State& state = mRegistry->GetComponent<Components::State>(entity);
	//Components::Seek& seek = mRegistry->GetComponent<Components::Seek>(entity);
	//Components::Transform& transform = mRegistry->GetComponent<Components::Transform>(entity);


	Decision* wander = new Decision
	{
		[this](Entity entity) {
			Components::Seek& seek = mRegistry->GetComponent<Components::Seek>(entity);
			Components::Transform& transform = mRegistry->GetComponent<Components::Transform>(entity);

			if (seek.target.size() == 0) {
				int nearestRow = transform.position.y / mCellHeight;
				int nearestCol = transform.position.x / mCellWidth;
				std::vector<Entity> canadates;
				for (int i = 0; i < mRows; i++)
				{
					for (int j = 0; j < mCols; j++)
					{
						Entity cell = mGrid[i][j];
						Components::Tile& tile = mRegistry->GetComponent<Components::Tile>(cell);
						Components::Node& node = mRegistry->GetComponent<Components::Node>(cell);
						if (tile.type == TILETYPE::EMPTY)
						{
							canadates.push_back(cell);
						}
					}
				}

				int randIndex = rand() % canadates.size();
				ResetAstarGrid();
				std::vector<Entity> path = mAStar->Search(mGrid[nearestRow][nearestCol], canadates[randIndex]);
				seek.target = path;
			}
		}
	};

	Decision* findFood = new Decision
	{
		[this](Entity entity) {
			Components::Seek& seek = mRegistry->GetComponent<Components::Seek>(entity);
			Components::Transform& transform = mRegistry->GetComponent<Components::Transform>(entity);

			if (seek.target.size() == 0) {
				int nearestRow = transform.position.y / mCellHeight;
				int nearestCol = transform.position.x / mCellWidth;
				std::vector<Entity> canadates;
				for (int i = 0; i < mRows; i++)
				{
					for (int j = 0; j < mCols; j++)
					{
						Entity cell = mGrid[i][j];
						Components::Tile& tile = mRegistry->GetComponent<Components::Tile>(cell);
						Components::Transform& cellTransform = mRegistry->GetComponent<Components::Transform>(cell);
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
					ResetAstarGrid();
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

	Decision* foodNotInRange = new Decision
	{
		[this](Entity entity)
		{
			Components::DecisionTree& r = mRegistry->GetComponent<Components::DecisionTree>(entity);
			Decision* current = r.root->mFalseBranch->mFalseBranch->mFalseBranch;
			if (current->mCondition(entity))
			{
				current->mTrueBranch->mDecision(entity);
			}
			else
			{
				current->mFalseBranch->mDecision(entity);
			}
		},
		[this](Entity entity)
		{
			Components::AnimalStat& animalStat = mRegistry->GetComponent<Components::AnimalStat>(entity);
			Components::Transform& transform = mRegistry->GetComponent<Components::Transform>(entity);
			float minDist = FLT_MAX;
			for (int i = 0; i < mRows; i++)
			{
				for (int j = 0; j < mCols; j++)
				{
					Entity cell = mGrid[i][j];
					Components::Tile& tile = mRegistry->GetComponent<Components::Tile>(cell);
					Components::Transform& cellTransform = mRegistry->GetComponent<Components::Transform>(cell);
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
			return animalStat.hunger < 70 && minDist > 60.0f;
		},
		findFood,
		wander
	};

	Decision* eatFood = new Decision
	{
		[this](Entity entity)
		{
			Components::AnimalStat& animalStat = mRegistry->GetComponent<Components::AnimalStat>(entity);
			animalStat.eating = true;
		}
	};

	Decision* foodInRange = new Decision
	{
		[this](Entity entity)
		{
			Components::AnimalStat& animalStat = mRegistry->GetComponent<Components::AnimalStat>(entity);

			Components::DecisionTree& r = mRegistry->GetComponent<Components::DecisionTree>(entity);
			Decision* current = r.root->mFalseBranch->mFalseBranch;
			if (current->mCondition(entity))
			{
				current->mTrueBranch->mDecision(entity);
			}
			else
			{
				animalStat.eating = false;
				current->mFalseBranch->mDecision(entity);
			}
		},
		[this](Entity entity)
		{
			Components::AnimalStat& animalStat = mRegistry->GetComponent<Components::AnimalStat>(entity);
			Components::Transform& transform = mRegistry->GetComponent<Components::Transform>(entity);

			float minDist = FLT_MAX;
			for (int i = 0; i < mRows; i++)
			{
				for (int j = 0; j < mCols; j++)
				{
					Entity cell = mGrid[i][j];
					Components::Tile& tile = mRegistry->GetComponent<Components::Tile>(cell);
					Components::Transform& cellTransform = mRegistry->GetComponent<Components::Transform>(cell);
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
			return (animalStat.eating && animalStat.hunger < 100.0f) || (animalStat.hunger < 70 && minDist <= 60.0f);
		},
		eatFood,
		foodNotInRange
	};

	Decision* findWater = new Decision
	{
		[this](Entity entity) {
			Components::Seek& seek = mRegistry->GetComponent<Components::Seek>(entity);
			Components::Transform& transform = mRegistry->GetComponent<Components::Transform>(entity);
			if (seek.target.size() == 0) {
				int nearestRow = transform.position.y / mCellHeight;
				int nearestCol = transform.position.x / mCellWidth;
				std::vector<Entity> canadates;
				for (int i = 0; i < mRows; i++)
				{
					for (int j = 0; j < mCols; j++)
					{
						Entity cell = mGrid[i][j];
						Components::Tile& tile = mRegistry->GetComponent<Components::Tile>(cell);
						Components::Transform& cellTransform = mRegistry->GetComponent<Components::Transform>(cell);
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
					ResetAstarGrid();
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

	Decision* waterNotInRange = new Decision
	{
		[this](Entity entity)
		{
			Components::DecisionTree& r = mRegistry->GetComponent<Components::DecisionTree>(entity);
			Decision* current = r.root->mFalseBranch;
			if (current->mCondition(entity))
			{
				current->mTrueBranch->mDecision(entity);
			}
			else
			{
				current->mFalseBranch->mDecision(entity);
			}
		},
		[this](Entity entity)
		{
			Components::AnimalStat& animalStat = mRegistry->GetComponent<Components::AnimalStat>(entity);
			Components::Transform& transform = mRegistry->GetComponent<Components::Transform>(entity);
			float minDist = FLT_MAX;
			for (int i = 0; i < mRows; i++)
			{
				for (int j = 0; j < mCols; j++)
				{
					Entity cell = mGrid[i][j];
					Components::Tile& tile = mRegistry->GetComponent<Components::Tile>(cell);
					Components::Transform& cellTransform = mRegistry->GetComponent<Components::Transform>(cell);
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
			return animalStat.thirst < 70 && minDist > 60.0f && animalStat.thirst <= animalStat.hunger;
		},
		findWater,
		foodInRange
	};


	Decision* drinkWater = new Decision
	{
		[this](Entity entity)
		{
			Components::AnimalStat& animalStat = mRegistry->GetComponent<Components::AnimalStat>(entity);
			animalStat.drinking = true;
		}
	};

	Decision* root = new Decision 
	{
		[this](Entity entity) {
			Components::AnimalStat& animalStat = mRegistry->GetComponent<Components::AnimalStat>(entity);
			Components::DecisionTree& r = mRegistry->GetComponent<Components::DecisionTree>(entity);
			if (r.root->mCondition(entity))
			{
				r.root->mTrueBranch->mDecision(entity);
			}
			else
			{
				animalStat.drinking = false;
				r.root->mFalseBranch->mDecision(entity);
			}
		},
		[this](Entity entity) {
			Components::AnimalStat& animalStat = mRegistry->GetComponent<Components::AnimalStat>(entity);
			Components::Transform& transform = mRegistry->GetComponent<Components::Transform>(entity);

			float minDist = FLT_MAX;
			for (int i = 0; i < mRows; i++)
			{
				for (int j = 0; j < mCols; j++)
				{
					Entity cell = mGrid[i][j];
					Components::Tile& tile = mRegistry->GetComponent<Components::Tile>(cell);
					Components::Transform& cellTransform = mRegistry->GetComponent<Components::Transform>(cell);
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
			return (animalStat.drinking && animalStat.thirst < 100.0f) || (animalStat.thirst < 70 && minDist <= 60.0f && animalStat.thirst <= animalStat.hunger);
		},
		drinkWater,
		waterNotInRange
	};

	mRegistry->AddComponent(
		entity,
		Components::DecisionTree
		{
			root
		}
	);
	return root;
}

void Game::ResetAstarGrid()
{
	for (int i = 0; i < mRows; i++)
	{
		for (int j = 0; j < mCols; j++)
		{
			Entity cell = mGrid[i][j];
			Components::Node& node = mRegistry->GetComponent<Components::Node>(cell);
			node.previous = -1;
		}
	}
}

