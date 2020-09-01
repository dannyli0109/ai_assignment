#pragma once
#include "Registry.h"
#include "TextureManager.h"

#include "Rect.h"
#include "Sprite.h"
#include "Transform.h"
#include "RigidBody.h"
#include "SpriteAnimation.h"
#include "Decision.h"
#include "Tile.h"

#include "SpriteRenderer.h"
#include "SpriteAnimator.h"
#include "Physics.h"
#include "Tilemap.h"

#include <vector>
#include <sstream>
#include <fstream>

class Game
{
public:
	Game() = default;
	void Init();
	void Update();
	void Draw();
	void Shutdown();
private:
	void RegisterComponents();
	void RegisterSystems();
	void CreateTextureManager();
	Entity CreateGridCell(int i, int j);
	void CreateChicken();
	void LoadTileMap();
	void CreateDecisionTree();
private:
	std::vector<std::vector<Entity>> mGrid;
	unsigned int mRows;
	unsigned int mCols;
	float mCellWidth;
	float mCellHeight;
	float mDeltaTime;
	Registry* mRegistry;

	Systems::SpriteRenderer* mSpriteRenderer;
	Systems::Physics* mPhysics;
	Systems::SpriteAnimator* mSpriteAnimator;
	Systems::TileMap* mTileMap;
};