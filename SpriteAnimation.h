#pragma once
#include <vector>
#include <unordered_map>
#include <string>
#include <functional>
namespace Components
{
	struct SpriteAnimation
	{
		std::unordered_map<std::string, std::vector<int>> frames;
		float speed;
		std::function<void(Entity)> transitions = [](Entity entity) {};
		std::string state = "idle";
		int frame = 0;
		float time = 0.0f;
	};
}