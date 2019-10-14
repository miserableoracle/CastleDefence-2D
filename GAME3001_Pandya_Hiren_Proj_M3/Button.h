/****************************************
Filename:		Button.h
Author:			Prof. Alexander Richard
Modified By:	Hiren Pandya (101198481)
Created On:		10/03/2019
Modified On:	25/03/2019
*****************************************/
#pragma once
#include "pch.h"
#include "SDL.h"

class Button
{
public:
	Button(const char* s, SDL_Rect d);
	Button(const char*, SDL_Rect, SDL_Rect);
	Button(const char* s, SDL_Rect d, const char* hoverSound, const char* clickSound);
	~Button();
	void Update();
	void Render();
	bool Clicked() { return m_iFrame == 3; }
	void PlaySound(int);
	void ReleaseClick();
	void SetButtonState(bool);

private:
	enum state { MOUSE_UP, MOUSE_DIS, MOUSE_OVER, MOUSE_DOWN };
	bool m_bSoundSet;
	bool m_bHoverReleased;
	bool m_bReleased;
	bool m_bIsDisabled;
	int m_iFrame;
	SDL_Rect m_rSrc;
	SDL_Rect m_rDst;
	SDL_Surface* m_pSurf;
	SDL_Texture* m_pText;
};