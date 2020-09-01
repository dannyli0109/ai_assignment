#pragma once
#include "System.h"
#include "Registry.h"
#include "Tile.h"
#include "Sprite.h"
#include "TextureManager.h"
#include <sstream>

namespace Systems {

	class TileMap : public System
	{
		std::vector<std::string> tileNames{
			"",
			"floor_wood",
			"wall"
		};
	public:
		void Init()
		{
			for (Entity entity : mEntities)
			{
				Components::Tile& tile = Registry::GetInstance()->GetComponent<Components::Tile>(entity);
				Components::Sprites& sprites = Registry::GetInstance()->GetComponent<Components::Sprites>(entity);
				Components::Sprite& sprite = sprites.elements[sprites.index];

				std::stringstream ss;
				ss << tileNames[int(tile.type)] << "_" << tile.mask.to_ulong();
				sprite.texture = TextureManager::GetInstance()->Get(ss.str());
			}
		}
	};
}
