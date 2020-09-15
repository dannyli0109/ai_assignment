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
	public:
		// initialise the tilemap
		void Init()
		{
			for (Entity entity : mEntities)
			{
				Components::Tile& tile = Registry::GetInstance()->GetComponent<Components::Tile>(entity);
				Components::Sprites& sprites = Registry::GetInstance()->GetComponent<Components::Sprites>(entity);
				Components::Sprite& sprite = sprites.elements[sprites.index];

				switch (tile.type)
				{
				case TILETYPE::EMPTY:
				{
					sprites.elements = {
						Components::Sprite
						{
							TextureManager::GetInstance()->Get("pixel_pack"),
							6 * 16, 2 * 16,
							16, 16
						}
					};
					break;
				}
				case TILETYPE::FLOOR:
					break;
				case TILETYPE::WALL:
				{
					std::stringstream ss;
					ss << "wall_" << tile.mask.to_ulong();
					sprite.texture = TextureManager::GetInstance()->Get(ss.str());
					break;
				}
				case TILETYPE::WATER:
				{
					sprites.elements = {
						Components::Sprite
						{
							TextureManager::GetInstance()->Get("pixel_pack"),
							0, 128,
							16, 16
						},
						Components::Sprite
						{
							TextureManager::GetInstance()->Get("pixel_pack"),
							16, 128,
							16, 16
						},
						Components::Sprite
						{
							TextureManager::GetInstance()->Get("pixel_pack"),
							32, 128,
							16, 16
						},
						Components::Sprite
						{
							TextureManager::GetInstance()->Get("pixel_pack"),
							48, 128,
							16, 16
						}
					};

					Registry::GetInstance()->AddComponent(
						entity,
						Components::SpriteAnimation
						{
							{
								{"idle", {0, 1, 2, 3}},
							},
							0.4f
						}
					);
					break;
				}
				case TILETYPE::FOOD:
				{
					sprites.elements = {
						Components::Sprite
						{
							TextureManager::GetInstance()->Get("pixel_pack"),
							9 * 16, 2 * 16,
							16, 16
						}
					};

					break;
				}
				default:
					break;
				}

			}
		}
	};
}
