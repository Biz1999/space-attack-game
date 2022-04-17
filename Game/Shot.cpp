#include "Shot.h"
#include "AnimSpriteComponent.h"
#include "Asteroid.h"

const float WINDOW_WIDTH = 1024.0f;
const float ASTEROID_WIDTH = 80.0f;
const float ASTEROID_HEIGHT = 80.0f;
const float SHOT_WIDTH = 131.0f;
const float SHOT_HEIGHT = 79.0f;

Shot::Shot(Game* game)
	:Actor(game)
	,mSpeed(2000.0f)
	,count(0)
{

	AnimSpriteComponent* asc = new AnimSpriteComponent(this);

	std::vector<SDL_Texture*> anims = {
		game->GetTexture("Assets/shot_middle.png"),
		game->GetTexture("Assets/shot_middle2.png"),
	};

	asc->SetAnimTextures(anims);
}


void Shot::UpdateActor(float deltaTime)
{
	Actor::UpdateActor(deltaTime);
	// Update position based on speeds and delta time
	Vector2 pos = GetPosition();
	pos.x += mSpeed * deltaTime;
	pos.y = pos.y;
	
	if (this->isShotOffScreen())
		Actor::SetState(Actor::EDead);

	SetPosition(pos);
}

bool Shot::CollidesWithAsteroid(Asteroid* asteroid)
{
	float asteroidInitialHeight = asteroid->GetPosition().y - (ASTEROID_HEIGHT * asteroid->GetScale() / 2);
	float asteroidEndHeight = asteroid->GetPosition().y + (ASTEROID_HEIGHT * asteroid->GetScale() / 2);
	float asteroidInitialWidth = asteroid->GetPosition().x - (ASTEROID_WIDTH * asteroid->GetScale() / 2);
	float asteroidEndWidth = asteroid->GetPosition().x + (ASTEROID_WIDTH * asteroid->GetScale() / 2);


	float shotInitialHeight = this->GetPosition().y - (SHOT_HEIGHT * this->GetScale() / 2);
	float shotEndHeight = this->GetPosition().y + (SHOT_HEIGHT * this->GetScale() / 2);
	float shotInitialWidth = this->GetPosition().x - (SHOT_WIDTH * this->GetScale() / 2);
	float shotEndWidth = this->GetPosition().x + (SHOT_WIDTH * this->GetScale() / 2);

	if (shotEndHeight >= asteroidInitialHeight
		&& shotInitialHeight <= asteroidEndHeight
		&& shotEndWidth >= asteroidInitialWidth
		&& shotInitialWidth <= asteroidEndWidth) {
		this->SetState(Actor::EDead);
		return true;
	}

	return false;
}

bool Shot::isShotOffScreen()
{
	float shotScreenPosition = this->GetPosition().x;

	if (shotScreenPosition > WINDOW_WIDTH)
		return true;

	return false;
}

