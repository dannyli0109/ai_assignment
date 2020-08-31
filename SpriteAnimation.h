#pragma once
#include <vector>

namespace Components
{
	struct SpriteAnimation
	{
		std::vector<std::vector<int>> frames;
		float speed;
		int index = 0;
		int frame = 0;
		float time = 0.0f;
	};
}