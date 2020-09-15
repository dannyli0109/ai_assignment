#pragma once
#include "Mammal.h"
#include "Tile.h"
#include "Node.h"
#include "Vector3.h"
#include "Transform.h"
#include "Rigidbody.h"
#include "Seek.h";
#include "Avoid.h"
#include "Sprite.h"
#include "TextureManager.h"
#include "Rect.h"
#include "SpriteAnimation.h"
#include "AnimalStat.h"

#include "ChickenDecisionTree.h"
#include "AStar.h"


class Chicken : public Mammal
{
public:
	std::vector <std::vector<Entity>> mGrid;
	ChickenDecisionTree* mDecisionTree;
	Chicken(std::vector<std::vector<Entity>>& grid, float cellWidth, float cellHeight, Systems::AStar* aStar, std::vector<Entity>& enemies);
	virtual ~Chicken() override;
private:
	void Init();
	void PopulateObstacles();
};

