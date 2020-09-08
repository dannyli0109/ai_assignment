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
#include "Node.h"
#include "Seek.h"
#include "State.h"
#include "AnimalStat.h"
#include "Avoid.h"

#include "SpriteRenderer.h"
#include "SpriteAnimator.h"
#include "Physics.h"
#include "Tilemap.h"
#include "AStar.h"
#include "SeekBehavior.h"
#include "MakeDecision.h"
#include "Animal.h"
#include "AvoidBehavior.h"


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
	Entity CreateChicken();
	void LoadTileMap();
	void PopulateObstacles(Entity entity);
	Decision* CreateDecisionTree(Entity entity);
	void ResetAstarGrid();
private:
	std::vector<std::vector<Entity>> mGrid;
	unsigned int mRows;
	unsigned int mCols;
	float mCellWidth;
	float mCellHeight;
	float mDeltaTime;
	Registry* mRegistry;
	std::vector<Decision*> mDecisions;
	std::vector<Entity> mChickens;
	std::vector<std::vector<Obstacle*>> mObstacles;

	Systems::SpriteRenderer* mSpriteRenderer;
	Systems::Physics* mPhysics;
	Systems::SpriteAnimator* mSpriteAnimator;
	Systems::TileMap* mTileMap;
	Systems::AStar* mAStar;
	Systems::SeekBehavior* mSeekBehavior;
	Systems::MakeDecision* mMakeDecision;
	Systems::Animal* mAnimal;
	Systems::AvoidBehavior* mAvoidBehavior;
};