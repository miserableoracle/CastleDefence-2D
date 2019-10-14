#pragma once
#ifndef _ARROW_H_
#define _ARROW_H_
#include "LOTile.h"
#include <iostream>
using namespace std;
class Arrow : public LOTile {
public:
	int m_iMoveSpeed = 10;
	int m_iArrowType = 0;
	int m_iArrowCount = 0;
	int m_iFrameCtr = 0;
	int m_iFrameMax = 0;
	bool m_bRight = true;
	bool m_bHitEnemy = false;
	bool m_bPastMousePoint = false;
	bool m_bOutOfBounds = false;
	int m_iMouseX = 0;
	int m_iMouseY = 0;
	bool isActive = true;
	double m_Dx;
	double m_Dy;
	double m_dAngle;
	int m_iACount = 0;
	int m_iBlinkCounter = 0;

	Arrow() { }
	Arrow(int p_x, int p_y, int p_iMouseXPos, int p_iMouseYPos, SDL_Renderer* p_pRenderer, SDL_Texture* p_pTexture) : LOTile(p_x, p_y) {
		this->m_pRenderer = p_pRenderer;
		this->m_x = p_x;
		this->m_y = p_y;
		this->m_rSrc = {0, 0, 68, 14};
		this->SetImage(p_pRenderer, p_pTexture);
		updateDst();
		m_rDst.w = 68;
		m_rDst.h = 14;
		this->m_iMouseX = p_iMouseXPos;
		this->m_iMouseY = p_iMouseYPos;
	}

	void SetMouseLocation(int x, int y) {
		this->m_iMouseX = x;
		this->m_iMouseY = y;
	}

	void updateDst() {
		m_rDst.x = m_x;
		m_rDst.y = m_y;
	}

	void Animate() {
		//if(!this.)
		// For different arrow types
	}

	const SDL_Rect* GetSrc() { return &m_rSrc; }
	const SDL_Rect* GetDst() { return &m_rDst; }

	void Update() {
		if (!m_bHitEnemy) {
			if (!m_bPastMousePoint) {
				
				m_x += (int)(m_Dx * m_iMoveSpeed);
				m_y += (int)(m_Dy * m_iMoveSpeed);
				SDL_Rect* mouseBox = new SDL_Rect();
				mouseBox->x = m_iMouseX;
				mouseBox->y = m_iMouseY;
				mouseBox->h = 30;
				mouseBox->w = 30;
				//m_dAngle = atan2(m_iMouseY - m_y, m_iMouseX - m_x);
				//if (SDL_HasIntersection(this->GetDst(), mouseBox))
					//this->m_bPastMousePoint = true;
			}
			/*else {
				this->m_x += m_iMoveSpeed;
				this->m_y += m_iMoveSpeed;
				m_dAngle += 0.04;
			}*/

			if (this->m_x > 1024 || this->m_y > 768)
				m_bOutOfBounds = true;
		}
		updateDst();
	}
};

#endif  