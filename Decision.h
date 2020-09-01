#pragma once
#include <functional>

namespace Components
{
	struct Decision
	{
		Decision* mTrueBranch = nullptr;
		Decision* mFalseBranch = nullptr;
		std::function<bool(void)> mCondition = []() { return false; };
		std::function<void(void)> mDecision = []() {};
	};
}