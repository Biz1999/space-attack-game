#include "Asteroid.h"
#include "AnimSpriteComponent.h"
#include <stdlib.h> 

using namespace std;

const float ASTEROID_WIDTH = 125.0f;


Asteroid::Asteroid(Game* game)
	:Actor(game)
	,mRightSpeed(-300.0f)
{
	AnimSpriteComponent* asc = new AnimSpriteComponent(this);
	vector<string> asteroidStringSprites = {
		"Assets/asteroid-1.png",
		"Assets/asteroid-2.png",
		"Assets/asteroid-3.png",
		"Assets/asteroid-4.png",
		"Assets/asteroid-5.png"
	};
	vector<SDL_Texture*> anims = {
		game->GetTexture(asteroidStringSprites[rand() % 5]),
	};
	asc->SetAnimTextures(anims);
}

void Asteroid::UpdateActor(float deltaTime) {
	Actor::UpdateActor(deltaTime);
	Vector2 pos = GetPosition();
	pos.x += mRightSpeed * deltaTime;
	SetPosition(pos);

	if (this->isAsteroidOffScreen())
		this->SetState(Actor::EDead);
}

bool Asteroid::isAsteroidOffScreen()
{
	float asteroidScreenPosition = this->GetPosition().x + ASTEROID_WIDTH;

	if (asteroidScreenPosition < 0)
		return true;

	return false;
}
