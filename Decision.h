#pragma once
#include <functional>
#include "Registry.h"

struct Decision
{
	std::function<void()> mDecision = []() {};
	std::function<bool()> mCondition = []() { return true; };
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