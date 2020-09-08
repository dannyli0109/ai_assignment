#pragma once
#include <bitset>

enum class TILETYPE 
{
	EMPTY = 0,
	FLOOR,
	WALL,
	WATER,
	FOOD
};

namespace Components
{
	struct Tile
	{
		int row;
		int col;
		TILETYPE type;
		int sides;
		std::bitset<8> mask;
	};
}