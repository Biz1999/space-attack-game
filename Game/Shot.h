#pragma once
#include "Actor.h"
#include "Game.h"

class Shot : public Actor
{
public:
	Shot(class Game* game);
	void UpdateActor(float deltaTime) override;
	void ProcessKeyboard(const uint8_t* state) const {};
	float GetSpeed() const { return mSpeed; }

private:
	bool isShotOffScreen();
	float mSpeed;
	int count;

	

};