#pragma once
#include <set>
#include "Entity.h"
class System
{
public:
	std::set<Entity> mEntities;
	virtual void Update(float deltaTime) {};
	virtual void Draw() {};
};