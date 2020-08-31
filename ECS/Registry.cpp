#include "Registry.h"

Registry* Registry::mInstance = nullptr;
void Registry::Create()
{
	if (mInstance == nullptr)
		mInstance = new Registry;
}

void Registry::Destroy()
{
	delete mInstance;
	mInstance = nullptr;
}

Registry* Registry::GetInstance()
{
	return mInstance;
}

Registry::Registry()
{
	mComponentManager = new ComponentManager;
	mEntityManager = new EntityManager;
	mSystemManager = new SystemManager;
};

Registry::~Registry()
{
	delete mComponentManager;
	delete mEntityManager;
	delete mSystemManager;
}