/****************************************
Filename:		Button.cpp
Author:			Prof. Alexander Richard
Modified By:	Hiren Pandya (101198481)
Created On:		10/03/2019
Modified On:	25/03/2019
*****************************************/
#include "pch.h"
#include "Button.h"
#include "StateMachine.h"
#include "Game.h"
#include "SDL.h"
#include "SDL_image.h"

Button::Button(const char* s, SDL_Rect d)
{
	// Set the button image. You should have some fail checking just in case. 
	m_pSurf = IMG_Load(s);
	if (m_pSurf == nullptr) {
		cout << "[Button] Button Image Loading Failure." << endl;
		return;
	}
	m_pText = SDL_CreateTextureFromSurface(Game::Instance()->GetRenderer(), m_pSurf);
	SDL_FreeSurface(m_pSurf);
	// Do the rest.
	m_bReleased = true;
	m_rDst = d;
	m_rSrc.x = m_rSrc.y = 0;
	m_rSrc.w = m_rDst.w;
	m_rSrc.h = m_rDst.h;
	this->m_bSoundSet = false;
	m_bIsDisabled = false;
}

Button::Button(const char* s, SDL_Rect src, SDL_Rect dst)
{
	// Set the button image. You should have some fail checking just in case. 
	m_pSurf = IMG_Load(s);
	m_pText = SDL_CreateTextureFromSurface(Game::Instance()->GetRenderer(), m_pSurf);
	SDL_FreeSurface(m_pSurf);
	// Do the rest.
	m_bReleased = true;
	m_rSrc = src;
	m_rDst = dst;
	m_bIsDisabled = false;
}

Button::Button(const char* s, SDL_Rect d, const char* hoverSound, const char* clickSound)
{
	// Set the button image. You should have some fail checking just in case. 
	m_pSurf = IMG_Load(s);
	if (m_pSurf == nullptr) {
		cout << "[Button] Button Image Loading Failure." << endl;
		return;
	}
	m_pText = SDL_CreateTextureFromSurface(Game::Instance()->GetRenderer(), m_pSurf);
	SDL_FreeSurface(m_pSurf);
	Game::Instance()->GetAM()->LoadSound(hoverSound);
	Game::Instance()->GetAM()->LoadSound(clickSound);
	// Do the rest.
	m_bReleased = true;
	m_rDst = d;
	m_rSrc.x = m_rSrc.y = 0;
	m_rSrc.w = m_rDst.w;
	m_rSrc.h = m_rDst.h;
	this->m_bSoundSet = true;
	this->m_bHoverReleased = true;
	m_bIsDisabled = false;
}

Button::~Button()
{
	SDL_DestroyTexture(m_pText);
}

void Button::Update()
{
	if (this->m_bIsDisabled) {
		m_iFrame = MOUSE_DIS;
		return;
	}
	int mx = Game::Instance()->GetMouseX();
	int my = Game::Instance()->GetMouseY();
	if (mx < (m_rDst.x + m_rDst.w) && mx > m_rDst.x && // If cursor is within X bounds
		my < (m_rDst.y + m_rDst.h) && my > m_rDst.y)   // And cursor is within Y bounds
	{
		if (Game::Instance()->GetLeftMouse() && m_bReleased)
		{
			this->PlaySound(1);
			m_iFrame = MOUSE_DOWN;
			m_bReleased = false;
		}
		else if (!Game::Instance()->GetLeftMouse())
		{
			m_bReleased = true;
			m_iFrame = MOUSE_OVER;
			if (this->m_bSoundSet && this->m_bHoverReleased) {
				this->PlaySound(0);
				this->m_bHoverReleased = false;
			}
		}
	}
	else {
		m_iFrame = MOUSE_UP;
		this->m_bHoverReleased = true;
	}
}

void Button::ReleaseClick() {
	m_bReleased = true;
	m_iFrame = MOUSE_OVER;
}

void Button::SetButtonState(bool state)
{
	m_bIsDisabled = state;
}

void Button::Render()
{
	m_rSrc.y = m_rDst.h * m_iFrame;
	SDL_RenderCopy(Game::Instance()->GetRenderer(), m_pText, &m_rSrc, &m_rDst);
}

void Button::PlaySound(int idx) {
	Game::Instance()->GetAM()->PlaySound(idx);
}
