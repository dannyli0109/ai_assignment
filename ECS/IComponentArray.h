#pragma once
#include "Entity.h"
#include <array>
#include <unordered_map>
#include <assert.h>

class IComponentArray
{
public:
	virtual ~IComponentArray() = default;
	virtual void EntityDestroyed(Entity entity) = 0;
};

template<typename T>
class ComponentArray : public IComponentArray
{
public:
	void InsertData(Entity entity, T component)
	{
		//assert(mEntityToIndexMap.find(entity) == mEntityToIndexMap.end() && "Component added to same entity more than once");

		unsigned int index = mSize;
		mIndexToEntityMap[index] = entity;
		mEntityToIndexMap[entity] = index;
		mComponentArray[index] = component;
		++mSize;
	}

	void RemoveData(Entity entity)
	{
		//assert(mEntityToIndexMap.find(entity) != mEntityToIndexMap.end() && "Removing non-existent component.");

		unsigned int index = mEntityToIndexMap[entity];
		unsigned int last = mSize - 1;
		mComponentArray[index] = mComponentArray[last];
		Entity lastEntity = mIndexToEntityMap[last];
		mEntityToIndexMap[lastEntity] = index;
		mIndexToEntityMap[index] = lastEntity;
		mEntityToIndexMap.erase(entity);
		mIndexToEntityMap.erase(last);
		--mSize;
	}

	T& GetData(Entity entity) {
		//assert(mEntityToIndexMap.find(entity) != mEntityToIndexMap.end() && "Retrieving non-existent component.");

		unsigned int index = mEntityToIndexMap[entity];
		return mComponentArray[index];
	}

	void EntityDestroyed(Entity entity) override
	{
		if (mEntityToIndexMap.find(entity) != mEntityToIndexMap.end()) {
			RemoveData(entity);
		}
	}

private:
	std::array<T, MAX_ENTITIES> mComponentArray;
	std::unordered_map<Entity, unsigned int> mEntityToIndexMap;
	std::unordered_map<unsigned int, Entity> mIndexToEntityMap;
	unsigned int mSize = 0;
	
};