// ----------------------------------------------------------------
// From Game Programming in C++ by Sanjay Madhav
// Copyright (C) 2017 Sanjay Madhav. All rights reserved.
// 
// Released under the BSD License
// See LICENSE in root directory for full details.
// ----------------------------------------------------------------

#include "Game.h"
#include "SDL/SDL_image.h"
#include <algorithm>
#include "Actor.h"
#include "SpriteComponent.h"
#include "Ship.h"
#include "BGSpriteComponent.h"
#include "Score.h"
#include "Asteroid.h"


const float ASTEROID_WIDTH = 125.0f;
const float ASTEROID_HEIGHT = 115.0f;
const float SHOT_WIDTH = 131.0f;
const float SHOT_HEIGHT = 79.0f;

Game::Game()
:mWindow(nullptr)
,mRenderer(nullptr)
,mIsRunning(true)
,mUpdatingActors(false)
,scoreCount(0)
{
	
}

bool Game::Initialize()
{
	if (SDL_Init(SDL_INIT_VIDEO|SDL_INIT_AUDIO) != 0)
	{
		SDL_Log("Unable to initialize SDL: %s", SDL_GetError());
		return false;
	}
	
	mWindow = SDL_CreateWindow("Game Programming in C++ (Chapter 2)", 100, 100, 1024, 600, 0);
	if (!mWindow)
	{
		SDL_Log("Failed to create window: %s", SDL_GetError());
		return false;
	}
	
	mRenderer = SDL_CreateRenderer(mWindow, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	if (!mRenderer)
	{
		SDL_Log("Failed to create renderer: %s", SDL_GetError());
		return false;
	}
	
	if (IMG_Init(IMG_INIT_PNG) == 0)
	{
		SDL_Log("Unable to initialize SDL_image: %s", SDL_GetError());
		return false;
	}

	LoadData();

	mTicksCount = SDL_GetTicks();
	
	return true;
}

void Game::LoadData()
{
	// Create player's ship
	mShip = new Ship(this);
	mShip->SetPosition(Vector2(100.0f, 300.0f));
	mShip->SetScale(1.5f);

	// Create actor for the background (this doesn't need a subclass)
	Actor* temp = new Actor(this);
	temp->SetPosition(Vector2(512.0f, 300.0f));
	// Create the "far back" background
	BGSpriteComponent* bg = new BGSpriteComponent(temp);
	bg->SetScreenSize(Vector2(1024.0f, 600.0f));
	std::vector<SDL_Texture*> bgtexs = {
		GetTexture("Assets/Farback01.png"),
		GetTexture("Assets/Farback02.png")
	};
	bg->SetBGTextures(bgtexs);
	bg->SetScrollSpeed(-100.0f);
	// Create the closer background
	bg = new BGSpriteComponent(temp, 50);
	bg->SetScreenSize(Vector2(1024.0f, 600.0f));
	bgtexs = {
		GetTexture("Assets/Stars.png"),
		GetTexture("Assets/Stars.png")
	};
	bg->SetBGTextures(bgtexs);
	bg->SetScrollSpeed(-200.0f);

	if (!score.Initialize())
		mIsRunning = false;
}

void Game::RunLoop()
{
	while (mIsRunning)
	{
		ProcessInput();
		UpdateGame();
		GenerateOutput();
		CreateAsteroid();
	}
}

void Game::ProcessInput()
{

	const Uint8* state = SDL_GetKeyboardState(NULL);

	SDL_Event event;
	while (SDL_PollEvent(&event))
	{
		switch (event.type)
		{
			case SDL_QUIT:
				mIsRunning = false;
				break;
			case SDL_KEYUP:
				switch (event.key.keysym.sym)
				{
					case SDLK_RETURN:
					{
						mShip->ResetShotTime(state);
						break;
					}
					default:
						break;
				}
		}
	}
	
	if (state[SDL_SCANCODE_ESCAPE])
	{
		mIsRunning = false;
	}

	if (state[SDL_SCANCODE_KP_ENTER] || state[SDL_SCANCODE_RETURN]) {

		if (mShip->GetShotCount() == 10) {
			// Create ship's shot
			Vector2 pos = mShip->GetPosition();
			Shot* shot = new Shot(this);
			shot->SetPosition(Vector2(pos.x + 50, pos.y));
			shot->SetScale(0.5f);
			shots.push_back(shot);

			mShip->SetShotCount(0);
		}

		mShip->SetShotCount(1);
	}

	// Process ship input
	mShip->ProcessKeyboard(state);
}

void Game::UpdateGame()
{
	// Compute delta time
	// Wait until 16ms has elapsed since last frame
	while (!SDL_TICKS_PASSED(SDL_GetTicks(), mTicksCount + 16))
		;

	float deltaTime = (SDL_GetTicks() - mTicksCount) / 1000.0f;
	if (deltaTime > 0.05f)
	{
		deltaTime = 0.05f;
	}
	mTicksCount = SDL_GetTicks();

	// Update all actors
	mUpdatingActors = true;
	for (auto actor : mActors)
	{
		actor->Update(deltaTime);
	}
	mUpdatingActors = false;

	// Move any pending actors to mActors
	for (auto pending : mPendingActors)
	{
		mActors.emplace_back(pending);
	}
	mPendingActors.clear();
	
	for (int i = 0; i < asteroids.size(); i++) {
		for (int j = 0; j < shots.size(); j++) {
			this->CheckShotHit(asteroids[i], shots[j]);
		}
	}

	// Add any dead actors to a temp vector
	std::vector<Actor*> deadActors;
	for (auto actor : mActors)
	{
	
		if (actor->GetState() == Actor::EDead)
		{
			deadActors.emplace_back(actor);
		}
	}

	int it = 0;
	for (auto& asteroid : asteroids) {
		if (asteroid->GetState() == Actor::EDead)
			asteroids.erase(asteroids.begin() + it);

		it++;
	}

	it = 0;
	for (auto& shot : shots) {
		if (shot->GetState() == Actor::EDead)
			shots.erase(shots.begin() + it);

		it++;
	}

	// Delete dead actors (which removes them from mActors)
	for (auto actor : deadActors)
	{
		delete actor;
	}

	this->SecondsPassed = SDL_GetTicks() / 1000;
	this->scoreCount = this->asteroidsDestroyed + this->SecondsPassed;
}

void Game::GenerateOutput()
{
	SDL_SetRenderDrawColor(mRenderer, 0, 0, 0, 255);
	SDL_RenderClear(mRenderer);
	
	// Draw all sprite components
	for (auto sprite : mSprites)
	{
		sprite->Draw(mRenderer);
	}

	int decimal = scoreCount / 10;
	int numeral = scoreCount % 10;

	vector<Led> allLeds = score.checkDrawnLeds(decimal, numeral);

	SDL_SetRenderDrawColor(mRenderer, 255, 255, 0, 255);

	for (unsigned int i = 0; i < allLeds.size(); i++) {
		SDL_Rect led{
		static_cast<int>(allLeds[i].x),
		static_cast<int>(allLeds[i].y),
		static_cast<int>(allLeds[i].width),
		static_cast<int>(allLeds[i].height)
		};
		SDL_RenderFillRect(mRenderer, &led);
	}


	SDL_RenderPresent(mRenderer);
}

void Game::CreateAsteroid() {
	asteroidCount += 1;

	if (asteroidCount == 80) {
		float randHeigth = rand() % 600 + 0.0f;
		Asteroid* asteroid = new Asteroid(this);
		asteroid->SetPosition(Vector2(1200.0f, randHeigth));
		asteroid->SetScale(1);

		asteroids.push_back(asteroid);
		asteroidCount = 0;
	}
}

void Game::CheckAsteroidShip(Actor asteroid, Actor ship) {
	Vector2 posAsteroid = asteroid.GetPosition();
	Vector2 posShip = mShip->GetPosition();
	if (posAsteroid.x == posShip.x + 50) {
		printf("ta no local");
	}

}

void Game::CheckShotHit(Actor* asteroid, Actor* shot)
{
	float asteroidEndHeight = asteroid->GetPosition().y + (ASTEROID_HEIGHT * asteroid->GetScale());
	float shotEndHeight = shot->GetPosition().y + (SHOT_HEIGHT * shot->GetScale());
	float asteroidEndWidth = asteroid->GetPosition().x + (ASTEROID_WIDTH * asteroid->GetScale());
	float shotEndWidth = shot->GetPosition().x + (SHOT_WIDTH * shot->GetScale());

	if (shotEndHeight >= asteroid->GetPosition().y
		&& shot->GetPosition().y <= asteroidEndHeight
		&& shotEndWidth >= asteroid->GetPosition().x
		&& shot->GetPosition().x <= asteroidEndWidth
		) {
		asteroid->SetState(Actor::EDead);
		shot->SetState(Actor::EDead);
		asteroidsDestroyed++;
	}
		
}


void Game::UnloadData()
{
	// Delete actors
	// Because ~Actor calls RemoveActor, have to use a different style loop
	while (!mActors.empty())
	{
		delete mActors.back();
	}

	// Destroy textures
	for (auto i : mTextures)
	{
		SDL_DestroyTexture(i.second);
	}
	mTextures.clear();
}

SDL_Texture* Game::GetTexture(const std::string& fileName)
{
	SDL_Texture* tex = nullptr;
	// Is the texture already in the map?
	auto iter = mTextures.find(fileName);
	if (iter != mTextures.end())
	{
		tex = iter->second;
	}
	else
	{
		// Load from file
		SDL_Surface* surf = IMG_Load(fileName.c_str());
		if (!surf)
		{
			SDL_Log("Failed to load texture file %s", fileName.c_str());
			return nullptr;
		}

		// Create texture from surface
		tex = SDL_CreateTextureFromSurface(mRenderer, surf);
		SDL_FreeSurface(surf);
		if (!tex)
		{
			SDL_Log("Failed to convert surface to texture for %s", fileName.c_str());
			return nullptr;
		}

		mTextures.emplace(fileName.c_str(), tex);
	}
	return tex;
}

void Game::Shutdown()
{
	UnloadData();
	IMG_Quit();
	SDL_DestroyRenderer(mRenderer);
	SDL_DestroyWindow(mWindow);
	SDL_Quit();
}

void Game::AddActor(Actor* actor)
{
	// If we're updating actors, need to add to pending
	if (mUpdatingActors)
	{
		mPendingActors.emplace_back(actor);
	}
	else
	{
		mActors.emplace_back(actor);
	}
}

void Game::RemoveActor(Actor* actor)
{
	// Is it in pending actors?
	auto iter = std::find(mPendingActors.begin(), mPendingActors.end(), actor);
	if (iter != mPendingActors.end())
	{
		// Swap to end of vector and pop off (avoid erase copies)
		std::iter_swap(iter, mPendingActors.end() - 1);
		mPendingActors.pop_back();
	}

	// Is it in actors?
	iter = std::find(mActors.begin(), mActors.end(), actor);
	if (iter != mActors.end())
	{
		// Swap to end of vector and pop off (avoid erase copies)
		std::iter_swap(iter, mActors.end() - 1);
		mActors.pop_back();
	}
}

void Game::AddSprite(SpriteComponent* sprite)
{
	// Find the insertion point in the sorted vector
	// (The first element with a higher draw order than me)
	int myDrawOrder = sprite->GetDrawOrder();
	auto iter = mSprites.begin();
	for ( ;
		iter != mSprites.end();
		++iter)
	{
		if (myDrawOrder < (*iter)->GetDrawOrder())
		{
			break;
		}
	}

	// Inserts element before position of iterator
	mSprites.insert(iter, sprite);
}

void Game::RemoveSprite(SpriteComponent* sprite)
{
	// (We can't swap because it ruins ordering)
	auto iter = std::find(mSprites.begin(), mSprites.end(), sprite);
	mSprites.erase(iter);
}
