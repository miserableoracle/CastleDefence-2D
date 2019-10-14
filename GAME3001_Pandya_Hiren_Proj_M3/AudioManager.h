/****************************************
Filename:		AudioManager.h
Author:			Prof. Alexander Richard
Modified By:	Hiren Pandya (101198481)
Created On:		10/03/2019
Modified On:	25/03/2019
*****************************************/
#pragma once
#include "pch.h"
#include <iostream>
#include <vector>
#include "SDL_mixer.h"
using namespace std;

class AudioManager
{
public:
	void SetMusicVolume(int v);										// Music Volume updater function
	void PlayMusic(int idx = 0, int loops = -1);					// Music player function
	void PlaySound(int idx, int channel = -1, int loops = 0);		// Sound player function
	void LoadMusic(const char* s);									// Music Loader function
	void LoadSound(const char* s);									// Sound Loader function
	void ToggleMusic();												// Toggle music
	void ClearMusic();												// Clear music
	void ClearSounds();												// Clear sounds
private:
	vector<Mix_Music*> m_vMusic;									// Vector to hold music
	vector<Mix_Chunk*> m_vSounds;									// Vector to hold sounds
};