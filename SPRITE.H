#pragma once
#include "raylib.h"
#include <vector>

namespace Components
{
	struct Sprite
	{
		Texture* texture;
		float x;
		float y;
		float width;
		float height;
		Color color = WHITE;
	};

	struct Sprites
	{
		std::vector<Sprite> elements;
		int index = 0;
	};
};