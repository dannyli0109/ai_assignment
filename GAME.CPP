#include "Game.h"

void Game::Init()
{
	Registry::Create();
	mRegistry = Registry::GetInstance();
	RegisterComponents();
	RegisterSystems();

	CreateTextureManager();
	mRows = 18;
	mCols = 18;
	mCellWidth = 60;
	mCellHeight = 60;
	mGrid = std::vector<std::vector<Entity>>(mRows,	std::vector<Entity>(mCols, 0));

	for (int i = 0; i < mRows; i++)
	{
		for (int j = 0; j < mCols; j++)
		{
			mGrid[i][j] = CreateGridCell(i, j);
		}
	}

	Entity entity = mRegistry->CreateEntity();

	mRegistry->AddComponent(
		entity,
		Components::Transform{
			Math::Vector3(
				200.0f, 200.0f, 1.0f
			),
			0,
			1.0f
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
				{0, 1, 2, 3}
			},
			0.2f
		}
	);


	mDeltaTime = 0.0f;
} 

void Game::Update()
{
	mDeltaTime = GetFrameTime();
	mPhysics->Update(mDeltaTime);
	mSpriteAnimator->Update(mDeltaTime);
}

void Game::Draw()
{
	mSpriteRenderer->Draw();
}

void Game::Shutdown()
{
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
					TextureManager::GetInstance()->Get("wall_15"),
					0.0f, 0.0f,
					64.0f, 64.0f
				}
			}
		}
	);
	return entity;
}
