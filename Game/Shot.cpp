#include "Shot.h"
#include "AnimSpriteComponent.h"
#include "Game.h"


Shot::Shot(Game* game)
	:Actor(game)
	, mSpeed(500.0f),
	count(0)
{

	AnimSpriteComponent* asc = new AnimSpriteComponent(this);
	std::vector<SDL_Texture*> shotInitial = {
		game->GetTexture("Assets/shot_start.png"),
		game->GetTexture("Assets/shot_start2.png"),
	};

	std::vector<SDL_Texture*> shotMiddle = {
		game->GetTexture("Assets/shot_middle.png"),
		game->GetTexture("Assets/shot_middle2.png"),
	};


	std::vector<SDL_Texture*> shotCollision = {
		game->GetTexture("Assets/shot_final.png"),
		game->GetTexture("Assets/shot_final2.png"),
	};

	std::vector<SDL_Texture*> anims;




	if (count == 0) {
		anims = shotInitial;
		count = 1;
	} else if (count == 1) {
		anims = shotMiddle;
	}
	else {
		anims = shotCollision;
	}

	printf("%d\n", count);

	asc->SetAnimTextures(anims);

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


