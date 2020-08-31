#pragma once
#include "ComponentManager.h"
#include "SystemManager.h"
#include "EntityManager.h"
#include <vector>

class Registry
{
public:
	static void Create();
	static void Destroy();
	static Registry* GetInstance();

	Entity CreateEntity()
	{
		return mEntityManager->CreateEntity();
	}

	void DestroyEntity(Entity entity)
	{
		mEntityManager->DestroyEntity(entity);
		mComponentManager->EntityDestroyed(entity);
		mSystemManager->EntityDestroyed(entity);
	}

	template<typename T>
	void RegisterComponent()
	{
		mComponentManager->RegisterComponent<T>();
	}

	template<typename T>
	void AddComponent(Entity entity, T component)
	{
		mComponentManager->AddComponent<T>(entity, component);

		Signature signature = mEntityManager->GetSignature(entity);
		signature.set(mComponentManager->GetComponentType<T>(), true);
		mEntityManager->SetSignature(entity, signature);

		mSystemManager->EntitySignatureChanged(entity, signature);
	}

	template<typename T>
	void RemoveComponent(Entity entity)
	{
		mComponentManager->RemoveComponent<T>(entity);

		Signature signature = mEntityManager->GetSignature(entity);
		signature.set(mComponentManager->GetComponentType<T>(), false);
		mEntityManager->SetSignature(entity, signature);

		mSystemManager->EntitySignatureChanged(entity, signature);
	}

	template<typename T>
	T& GetComponent(Entity entity)
	{
		return mComponentManager->GetComponent<T>(entity);
	}

	template<typename T>
	ComponentType GetComponentType()
	{
		return mComponentManager->GetComponentType<T>();
	}

	template<typename T>
	T* RegisterSystem()
	{
		return mSystemManager->RegisterSystem<T>();
	}

	template<typename T>
	void SetSystemSignature(Signature signature)
	{
		mSystemManager->SetSignature<T>(signature);
	}

	template<typename T>
	void SetSystemSignatureV(const std::vector<ComponentType>& componentTypes)
	{
		Signature signature;
		for (ComponentType componentType : componentTypes) {
			signature.set(componentType);
		}
		mSystemManager->SetSignature<T>(signature);
	}

public:
	ComponentManager* mComponentManager;
	EntityManager* mEntityManager;
	SystemManager* mSystemManager;
private:
	Registry();
	~Registry();
	static Registry* mInstance;
};
