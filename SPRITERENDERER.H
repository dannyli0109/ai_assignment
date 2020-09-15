#pragma once
#include "Registry.h"
#include "System.h"
#include "raylib.h"
#include "Transform.h"
#include "Sprite.h"
#include "Rect.h"

namespace Systems {
	class SpriteRenderer : public System {
	public:
		// render the sprite using position and rect
		void Draw() override
		{
			for (Entity entity : mEntities)
			{
				const Components::Transform& transform = Registry::GetInstance()->GetComponent<Components::Transform>(entity);
				const Components::Sprites& sprites = Registry::GetInstance()->GetComponent<Components::Sprites>(entity);
				const Components::Rect& rect = Registry::GetInstance()->GetComponent<Components::Rect>(entity);

				if (sprites.elements.size() > sprites.index)
				{
					const Components::Sprite& sprite = sprites.elements[sprites.index];
					if (sprite.texture) {
						Rectangle sourceRec{ sprite.x, sprite.y, sprite.width, sprite.height };
						Rectangle destRec{ transform.position.x, transform.position.y, rect.width, rect.height };
						Vector2 origin{ rect.offX, rect.offY };
						DrawTexturePro(*sprite.texture, sourceRec, destRec, origin, transform.rotation, sprite.color);
					}
				}
			}
		}
	};
}