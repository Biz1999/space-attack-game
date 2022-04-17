#include "Shot.h"
#include "AnimSpriteComponent.h"

const float WINDOW_WIDTH = 1024.0f;
const float SHOT_WIDTH = 131.0f;

Shot::Shot(Game* game)
	:Actor(game)
	,mSpeed(1000.0f)
	,count(0)
{

	AnimSpriteComponent* asc = new AnimSpriteComponent(this);
	std::vector<SDL_Texture*> shotInitial = {
		game->GetTexture("Assets/shot_start.png"),
		game->GetTexture("Assets/shot_start2.png"),
		game->GetTexture("Assets/shot_middle.png"),
		game->GetTexture("Assets/shot_middle2.png")
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
		asc->SetAnimTextures(anims);
	} 

	if (count == 1) {
		anims = shotMiddle;
		asc->SetAnimTextures(anims);
	}
	else {
		anims = shotCollision;
	}

	//printf("%d\n", count);

	// asc->SetAnimTextures(anims);

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

bool Shot::isShotOffScreen()
{
	float shotScreenPosition = this->GetPosition().x;

	if (shotScreenPosition > WINDOW_WIDTH)
		return true;

	return false;
}

