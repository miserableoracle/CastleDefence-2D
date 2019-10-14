#pragma once
#ifndef _ENEMIES_H_
#define _ENEMIES_H_
#include "LOTile.h"

class Enemy : public LOTile {
public:
	int m_iMoveSpeed = 3;
	int m_iEnemyType = 0;
	int m_iFrameMin = 0;
	int m_iFrameCtr;
	int m_iFrameMax = 7;
	int m_iFrame = 0;
	int m_iMax = 6;
	int m_iTargetX = 0;
	int m_iTargetY = 0;
	bool reachDest = false;
	bool m_bRight = true;
	bool isAlive = true;
	bool isHit = false;
	SDL_Rect bounds;
	int m_iECount = 0;
	int m_iBlinkCounter = 0;

	Enemy() { }
	Enemy(int p_x, int p_y, int p_type, int p_targetX, int p_targetY, SDL_Renderer* p_pRenderer, SDL_Texture* p_pTexture) : LOTile(p_x, p_y) // Calling constructor of the super class
	{
		this->m_pRenderer = p_pRenderer; // Set the renderer
		this->m_iEnemyType = p_type;
		this->m_iTargetX = p_targetX;
		this->m_iTargetY = p_targetY;
		this->m_x = p_x; // X coordinate of enemy's location
		this->m_y = p_y; // Y coordinate of enemy's location
		if (p_type == 1) {
			this->m_rSrc = { 0, 192, 64, 64 }; // Set default properties of m_rSrc rect
			m_rDst.w = 64; // Set dest rect width
			m_rDst.h = 64; // Set dest rect height
		}
		else if (p_type == 2) {
			this->m_rSrc = { 0, 1056, 96, 96 }; // Set default properties of m_rSrc rect
			m_rDst.w = 96; // Set dest rect width
			m_rDst.h = 96; // Set dest rect height
		}
		this->SetImage(p_pRenderer, p_pTexture); // Set the enemy image
		updateDst(); // Update enemy's location on the map
		
		m_iFrameCtr = m_iFrameMin;

		if (p_type == 1) { // Normal goblin
			bounds = { m_rDst.x + 30, m_rDst.y + 20, (m_rDst.w / 2) - 10, (m_rDst.h / 2) + 10 };
		}
	}

	// Update enemy's location on the map
	void updateDst() {
		m_rDst.x = m_x;
		m_rDst.y = m_y;
	}
	// Move enemy by chaning X coordinate
	void MoveX(int p_iX) {
		if (!reachDest) {
			m_x += p_iX * m_iMoveSpeed;
			updateDst();
		}
		if (m_x < m_iTargetX) {
			if (m_iEnemyType == 1) {
				reachDest = true;
				m_iFrameMin = 8;
				m_iFrameMax = 11;
			}
			if (m_iEnemyType == 2) {
				reachDest = true;
				this->m_rSrc = { 0, 1536, 96, 96 };
				m_iFrameMin = 0;
				m_iFrameMax = 9;
			}
		}
	}
	// Move enemy by changing Y coordinate
	void MoveY(int p_iY) {
		m_y += p_iY * m_iMoveSpeed;
		updateDst();
	}

	void AdvanceAnim()
	{
		if (!this->isAlive)
			return;
		m_iFrame++;
		if (m_iFrame == m_iMax) {
			m_iFrame = 0;
			m_iFrameCtr++;
			if (m_iFrameCtr == m_iFrameMax)
			{
				m_iFrameCtr = m_iFrameMin; // Resetting frame counter.
			}
		}
		if(m_iEnemyType == 1)
			m_rSrc.x = 64 * m_iFrameCtr; // Prints appropriate 'cell.'
		if(m_iEnemyType == 2)
			m_rSrc.x = 96 * m_iFrameCtr; // Prints appropriate 'cell.'
	}
	
	// Getters for src and dest rect
	const SDL_Rect* GetSrc() { return &m_rSrc; }
	const SDL_Rect* GetDst() { return &m_rDst; }
	void Update() {
		AdvanceAnim();
	}
};

#endif