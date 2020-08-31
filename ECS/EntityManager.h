#pragma once
#include "Component.h"
#include "Entity.h"
#include <queue>
#include <array>
#include <assert.h>


class EntityManager {
public:
	EntityManager() 
	{
		for (Entity entity = 0; entity < MAX_ENTITIES; ++entity)
		{
			mAvailableEntities.push(entity);
		}
	}

	Entity CreateEntity()
	{
		//assert(mLivingEntityCount < MAX_ENTITIES && "Too many entities in existence.");

		Entity id = mAvailableEntities.front();
		mSignatures[id].reset();
		mAvailableEntities.pop();
		++mLivingEntityCount;
		return id;
	}

	void DestroyEntity(Entity entity)
	{
		//assert(entity < MAX_ENTITIES && "Entity out of range.");

		mSignatures[entity].reset();
		mAvailableEntities.push(entity);
		--mLivingEntityCount;
	}

	void SetSignature(Entity entity, Signature signature)
	{
		//assert(entity < MAX_ENTITIES && "Entity out of range.");

		mSignatures[entity] = signature;
	}

	Signature GetSignature(Entity entity)
	{
		//assert(entity < MAX_ENTITIES && "Entity out of range.");
		return mSignatures[entity];
	}
private:
	std::queue<Entity> mAvailableEntities;
	std::array<Signature, MAX_ENTITIES> mSignatures;
	unsigned int mLivingEntityCount = 0;
};