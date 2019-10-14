/****************************************
Filename:		AudioManager.cpp
Author:			Prof. Alexander Richard
Modified By:	Hiren Pandya (101198481)
Created On:		10/03/2019
Modified On:	25/03/2019
*****************************************/
#include "pch.h"
#include "AudioManager.h"

// Music volume updater function
void AudioManager::SetMusicVolume(int v)
{
	v = (v < 0 ? 0 : (v > MIX_MAX_VOLUME ? MIX_MAX_VOLUME : v)); // Clamps vol from 0 to 128. 
	Mix_VolumeMusic(v);
}

// Music player function
void AudioManager::PlayMusic(int idx, int loops)
{
	Mix_PlayMusic(m_vMusic[idx], loops);
}

// Sound player function
void AudioManager::PlaySound(int idx, int channel, int loops)
{
	Mix_PlayChannel(channel, m_vSounds[idx], loops);
}

// Music Loader function
void AudioManager::LoadMusic(const char* s)
{
	Mix_Music* t = Mix_LoadMUS(s);
	if (t != nullptr)
		m_vMusic.push_back(t);
	else cout << "[AudioManager] Music load fail" << endl;
}

// Sound loader function
void AudioManager::LoadSound(const char* s)
{
	Mix_Chunk* t = Mix_LoadWAV(s);
	if (t != nullptr)
		m_vSounds.push_back(t);
	else cout << "[AudioManager] Sound load fail" << endl;
}

// Music Toggler
void AudioManager::ToggleMusic()
{
	if (Mix_PausedMusic() == 1)
		Mix_ResumeMusic();
	else
		Mix_PauseMusic();
}

// Clear music function
void AudioManager::ClearMusic()
{
	for (int i = 0; i < (int)m_vMusic.size(); i++)
	{
		Mix_FreeMusic(m_vMusic[i]);
		m_vMusic[i] = nullptr;
	}
	m_vMusic.clear();
	m_vMusic.shrink_to_fit();
}

// Clear found function
void AudioManager::ClearSounds()
{
	for (int i = 0; i < (int)m_vSounds.size(); i++)
	{
		Mix_FreeChunk(m_vSounds[i]);
		m_vSounds[i] = nullptr;
	}
	m_vSounds.clear();
	m_vSounds.shrink_to_fit();
}