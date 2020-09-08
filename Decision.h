#pragma once
#include <functional>


struct Decision
{
	std::function<void(Entity)> mDecision = [](Entity entity) {};
	std::function<bool(Entity)> mCondition = [](Entity entity) { return true; };
	Decision* mTrueBranch = nullptr;
	Decision* mFalseBranch = nullptr;

	void DeleteChildren()
	{
		if (mTrueBranch)
		{
			mTrueBranch->DeleteChildren();
		}

		if (mFalseBranch)
		{
			mFalseBranch->DeleteChildren();
		}
		delete this;
	}
	//~Decision()
	//{
	//	DeleteChildren();
	//}
};

namespace Components
{
	struct DecisionTree
	{
		Decision* root;
		float interval;
		float time = 1.0f;
	};
}