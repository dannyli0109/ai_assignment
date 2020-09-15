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
		mMammal.push_back(new Chicken(mGrid, mCellWidth, mCellHeight, mAStar, mEnemies));
		mPreys.push_back(mMammal[i]->mEntity);
	}

	mMammal.push_back(new Llama(mGrid, mCellWidth, mCellHeight, mAStar, mPreys));
	mEnemies.push_back(mMammal[mMammal.size() - 1]->mEntity);

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
	//mAnimal->Debug();
	//mSeekBehavior->Debug();
}

void Game::Shutdown()
{
	for (Mammal* m : mMammal)
	{
		delete m;
	}
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
	tm->LoadTexture("chicken_eat", "resources/chicken_eat.png");
	tm->LoadTexture("llama_walk", "resources/llama_walk.png");
	tm->LoadTexture("llama_eat", "resources/llama_eat.png");
	tm->LoadTexture("pixel_pack", "resources/pixel_pack.png");
	tm->LoadTexture("animals", "resources/animals.png");
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