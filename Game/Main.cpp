// ----------------------------------------------------------------
// From Game Programming in C++ by Sanjay Madhav
// Copyright (C) 2017 Sanjay Madhav. All rights reserved.
// 
// Released under the BSD License
// See LICENSE in root directory for full details.
// ----------------------------------------------------------------

//Arthur Fabris Gatti			RA : 22.119.022 - 6
//Islan Silva Figueredo			RA : 22.119.027 - 5
//Lucas da Silva Oliveira		RA : 22.119.031 - 7
//Alessandro Biz				RA : 22.119.038 - 2
//Ivan Sanchez Tuzita			RA : 22.119.040 - 8

#include "Game.h"

int main(int argc, char** argv)
{
	Game game;
	bool success = game.Initialize();
	if (success)
	{
		game.RunLoop();
	}
	game.Shutdown();
	return 0;
}
