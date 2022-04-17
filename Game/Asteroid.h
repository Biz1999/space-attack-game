#pragma once
#include "Actor.h"
#include "Game.h"

class Asteroid : public Actor
{
public:
	Asteroid(class Game* game);
	void UpdateActor(float deltaTime) override;
	float GetRightSpeed() const { return mRightSpeed; }
	bool isAsteroidOffScreen();
	bool isAsteroidDestroyed();
	bool collidesWithShip(class Ship* ship);
private:
	float mRightSpeed;
	int life = 5;
};
