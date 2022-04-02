#include "Asteroid.h"
#include "AnimSpriteComponent.h"
#include "Game.h"

Asteroid::Asteroid(Game* game)
	:Actor(game)
	, mRightSpeed(0.0f)
{
	AnimSpriteComponent* asc = new AnimSpriteComponent(this);
	std::vector<SDL_Texture*> anims = {
		game->GetTexture("Assets/asteroid-1"),
		game->GetTexture("Assets/asteroid-2"),
		game->GetTexture("Assets/asteroid-3"),
		game->GetTexture("Assets/asteroid-4"),
		game->GetTexture("Assets/asteroid-5"),
	};
	asc->SetAnimTextures(anims);
}

void Asteroid::UpdateActor(float deltaTime) {
	Actor::UpdateActor(deltaTime);
	Vector2 pos = GetPosition();
	pos.x += mRightSpeed * deltaTime;
	if (pos.x < 0) {
		//destroy object
	}
	SetPosition(pos);
}

