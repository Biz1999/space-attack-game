#include "Asteroid.h"
#include "Ship.h"
#include "AnimSpriteComponent.h"
#include <stdlib.h> 
#include <math.h>

using namespace std;

const float ASTEROID_WIDTH = 105.0f;
const float ASTEROID_HEIGHT = 80.0f;
const float SHIP_WIDTH = 64.0f;
const float SHIP_HEIGHT = 29.0f;


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

bool Asteroid::isAsteroidDestroyed()
{
	if (this->life == 1) {
		this->SetState(Actor::EDead);
		return true;
	}

	this->life--;
	return false;
}

bool Asteroid::collidesWithShip(Ship* ship)
{
	float asteroidInitialHeight = this->GetPosition().y - (ASTEROID_HEIGHT * this->GetScale() / 2);
	float asteroidEndHeight = this->GetPosition().y + (ASTEROID_HEIGHT * this->GetScale() / 2);
	float asteroidInitialWidth = this->GetPosition().x - (ASTEROID_WIDTH * this->GetScale() / 2);
	float asteroidEndWidth = this->GetPosition().x + (ASTEROID_WIDTH * this->GetScale() / 2);

	float shipInitialHeight = ship->GetPosition().y - (SHIP_HEIGHT * ship->GetScale() / 2);
	float shipEndHeight = ship->GetPosition().y + (SHIP_HEIGHT * ship->GetScale() / 2);
	float shipInitialWidth = ship->GetPosition().x - (SHIP_WIDTH * ship->GetScale() / 2);
	float shipEndWidth = ship->GetPosition().x + (SHIP_WIDTH * ship->GetScale() / 2);

	return (shipEndHeight >= asteroidInitialHeight
		&& shipInitialHeight <= asteroidEndHeight
		&& shipEndWidth >= asteroidInitialWidth
		&& shipInitialWidth <= asteroidEndWidth);
}
