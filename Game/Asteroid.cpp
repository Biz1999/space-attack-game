#include "Asteroid.h"
#include "AnimSpriteComponent.h"
#include "Game.h"
#include <stdlib.h> 

using namespace std;
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
	if (pos.x < 0) {
		//destroy object
	}
	SetPosition(pos);
}