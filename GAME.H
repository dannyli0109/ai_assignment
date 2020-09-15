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

#include "Chicken.h"
#include "Llama.h"

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
	// register components
	void RegisterComponents();
	// register systems and define components for each systems
	void RegisterSystems();
	// create singleton for texture manager
	void CreateTextureManager();
	// create the graph for the grid
	Entity CreateGridCell(int i, int j);
	// load the tilemap from data file
	void LoadTileMap();
private:
	std::vector<std::vector<Entity>> mGrid;
	unsigned int mRows;
	unsigned int mCols;
	float mCellWidth;
	float mCellHeight;
	float mDeltaTime;
	Registry* mRegistry;
	std::vector<Decision*> mDecisions;
	std::vector<Mammal*> mMammal;
	std::vector<Entity> mPreys;
	std::vector<Entity> mEnemies;

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