/***********************************
Filename:		GAME3001_Pandya_Hiren_Proj_M3.cpp
Author:			Hiren Pandya (101198481)
Created On:		10/03/2019
Modified On: 25/03/2019
************************************/
#include "pch.h"
#include <iostream>
#include "Game.h"
#include "Camera.h"
#include "SDL.h"
#include "SDL_image.h"
#include "SDL_mixer.h"
#include "SDL_ttf.h"
using namespace std;

int main(int argc, char* args[])
{
	Game::Instance()->Init("GAME3001_Pandya_Hiren_Proj_M3", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, Camera::width, Camera::height, 0);
	while (Game::Instance()->Running())
	{
		Game::Instance()->HandleEvents();
		if (Game::Instance()->Tick())
		{
			Game::Instance()->Update();
			Game::Instance()->Render();
		}
	}
	Game::Instance()->Clean();
	return 0;
}