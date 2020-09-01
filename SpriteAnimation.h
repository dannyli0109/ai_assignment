#pragma once
#include <vector>
#include <unordered_map>
#include <string>
namespace Components
{
	struct SpriteAnimation
	{
		std::unordered_map<std::string, std::vector<int>> frames;
		float speed;
		std::string state = "idle";
		int frame = 0;
		float time = 0.0f;
	};
}