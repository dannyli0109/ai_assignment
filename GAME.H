#pragma once
#include "Registry.h"
#include "TextureManager.h"

#include "Rect.h"
#include "Sprite.h"
#include "Transform.h"
#include "RigidBody.h"
#include "SpriteAnimation.h"

#include "SpriteRenderer.h"
#include "SpriteAnimator.h"
#include "Physics.h"

#include <vector>
#include <sstream>

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
private:
	Entity CreateGridCell(int i, int j);
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
};