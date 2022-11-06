#define SDL_MAIN_HANDLED
#include "Game.h"

/*  main.cpp
	Project Name - Frankenstein Game Engine
	Author - Frank Mock
	Description - Simple Game Engine
	Start Date  - 1/27/2018
	Last update - 11/05/2022
*/

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