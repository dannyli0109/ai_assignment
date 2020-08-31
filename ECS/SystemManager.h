#pragma once
#include <assert.h>
#include <unordered_map>
#include "Component.h"
#include "System.h"
class SystemManager
{
public:
	template<typename T>
	T* RegisterSystem() {
		const char* typeName = typeid(T).name();

		//assert(mSystems.find(typeName) == mSystems.end() && "Registering system more than once.");

		T* system = new T;
		mSystems.insert({ typeName, system });
		return system;
	}

	template<typename T>
	void SetSignature(Signature signature)
	{
		const char* typeName = typeid(T).name();

		//assert(mSystems.find(typeName) != mSystems.end() && "System used before registered.");

		mSignatures.insert({ typeName, signature });
	}

	void EntityDestroyed(Entity entity)
	{
		for (std::pair<const char*, System*> pair : mSystems)
		{
			System* system = pair.second;
			system->mEntities.erase(entity);
		}
	}

	void EntitySignatureChanged(Entity entity, Signature entitySignature)
	{
		for (std::pair<const char*, System*> pair : mSystems)
		{
			const char* type = pair.first;
			System* system = pair.second;
			Signature systemSignature = mSignatures[type];

			if ((entitySignature & systemSignature) == systemSignature)
			{
				system->mEntities.insert(entity);
			}
			else
			{
				system->mEntities.erase(entity);
			}
		}
	}

	~SystemManager()
	{
		for (std::pair<const char*, System*> pair : mSystems)
		{
			System* system = pair.second;
			delete system;
		}
	}
private:
	std::unordered_map<const char*, Signature> mSignatures;
	std::unordered_map<const char*, System*> mSystems;
};