// ----------------------------------------------------------------
// From Game Programming in C++ by Sanjay Madhav
// Copyright (C) 2017 Sanjay Madhav. All rights reserved.
// 
// Released under the BSD License
// See LICENSE in root directory for full details.
// ----------------------------------------------------------------

#pragma once
#include "Actor.h"
#include "Shot.h"
class Ship : public Actor
{
public:
	Ship(class Game* game);
	void UpdateActor(float deltaTime) override;
	void ProcessKeyboard(const uint8_t* state);
	void ResetShotTime(const uint8_t* state);
	void SetShotCount(int shotCount);
	float GetRightSpeed() const { return mRightSpeed; }
	float GetDownSpeed() const { return mDownSpeed; }
	int GetShotCount() const { return shotCount; }
private:
	float mRightSpeed;
	float mDownSpeed;
	Game* pointerGame;
	int shotCount;

	// Game-specific
	class Shot* mShot; //Actor Shot
};