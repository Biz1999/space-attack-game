#include "Shot.h"
#include "AnimSpriteComponent.h"
#include "Game.h"


Shot::Shot(Game* game)
	:Actor(game)
	, mSpeed(50.0f)
{
}


void Shot::UpdateActor(float deltaTime)
{
	Actor::UpdateActor(deltaTime);
	// Update position based on speeds and delta time
	Vector2 pos = GetPosition();
	pos.x += mSpeed * deltaTime;
	pos.y = pos.y;
	// Restrict position to left half of screen
	if (pos.x > 1024)
	{
		Actor::SetState(Actor::EDead);
	}
	
	SetPosition(pos);
}


void Shot::ProcessKeyboard(const uint8_t* state)
{
	
}


