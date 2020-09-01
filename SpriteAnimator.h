#pragma once
#include "Registry.h"
#include "System.h"
#include "Sprite.h"
#include "SpriteAnimation.h"

namespace Systems {
	class SpriteAnimator : public System {
	public:
		void Update(float deltaTime)
		{
			for (Entity entity : mEntities)
			{
				Components::Sprites& sprites = Registry::GetInstance()->GetComponent<Components::Sprites>(entity);
				Components::SpriteAnimation& spriteAnimation = Registry::GetInstance()->GetComponent<Components::SpriteAnimation>(entity);

				spriteAnimation.time += deltaTime;

				if (spriteAnimation.time >= spriteAnimation.speed)
				{
					spriteAnimation.time -= spriteAnimation.speed;
					std::vector<int> currentAnimation = spriteAnimation.frames[spriteAnimation.state];
					spriteAnimation.frame = (spriteAnimation.frame + 1) % currentAnimation.size();
					int nextIndex = currentAnimation[spriteAnimation.frame];
					sprites.index = nextIndex;
				}
			}
		}
	};
}