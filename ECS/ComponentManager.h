#pragma once
#include <unordered_map>
#include "Component.h"
#include "IComponentArray.h"
#include <memory>

class ComponentManager
{
public:
	template<typename T>
	void RegisterComponent()
	{
		const char* typeName = typeid(T).name();

		//assert(mComponentTypes.find(typeName) == mComponentTypes.end() && "Registering component type more than once.");

		mComponentTypes.insert({ typeName, mNextComponentType });
		mComponentArrays.insert({ typeName, new ComponentArray<T>() });
		++mNextComponentType;
	}

	template<typename T>
	ComponentType GetComponentType()
	{
		const char* typeName = typeid(T).name();

		//assert(mComponentTypes.find(typeName) != mComponentTypes.end() && "Component not registered before use.");

		return mComponentTypes[typeName];
	}

	template<typename T>
	void AddComponent(Entity entity, T component)
	{
		GetComponentArray<T>()->InsertData(entity, component);
	}

	template<typename T>
	void RemoveComponent(Entity entity)
	{
		GetComponentArray()->RemoveData(entity);
	}

	template<typename T>
	T& GetComponent(Entity entity)
	{
		return GetComponentArray<T>()->GetData(entity);
	}

	void EntityDestroyed(Entity entity) {
		for (std::pair<const char*, IComponentArray*> const& pair : mComponentArrays) {
			IComponentArray* const& component = pair.second;
			component->EntityDestroyed(entity);
		}
	}

	~ComponentManager() {
		for (std::pair<const char*, IComponentArray*> const& pair : mComponentArrays) {
			IComponentArray* const& component = pair.second;
			delete component;
		}
	}

private:
	std::unordered_map<const char*, ComponentType> mComponentTypes;
	std::unordered_map<const char*, IComponentArray*> mComponentArrays;
	ComponentType mNextComponentType = 0;

	template<typename T>
	ComponentArray<T>* GetComponentArray()
	{
		const char* typeName = typeid(T).name();

		//assert(mComponentTypes.find(typeName) != mComponentTypes.end() && "Component not registered before use.");

		return (ComponentArray<T>*)(mComponentArrays[typeName]);
	}
};