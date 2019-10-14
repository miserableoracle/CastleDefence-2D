/* File: Player.h
Author: Hiren Pandya (101198481)
Created On: 10/03/2019
Modified On: 25/03/2019
*/
#pragma once
#ifndef _PLAYER_H_
#define _PLAYER_H_
#include "LOTile.h"

/* Important note: x coordinate is player's COLUMN position and
y coordinate is player's ROW position. Don't mix these up! */

class Player : public LOTile
{
public:
	int m_iMoveSpeed = 2;
	int m_iFrameCtr = 0;
	int m_iFrameMax = 3;
	int m_iFrame = 0;
	int m_iMax = 6;
	bool m_bRight = true;
	bool isPlayerAlive = true;
	bool keyFound = false;
	bool swordFound = false;
	bool doAnimate = false;
	bool AnimStart = false;
	bool AnimFinish = false;
	enum animationFrames { InitFrame, Frame2, Frame3, LastFrame };
	// Default constructor
	Player() { }
	// Parameterised constructor
	Player(int p_x, int p_y, SDL_Renderer* p_pRenderer, SDL_Texture* p_pTexture) : LOTile(p_x, p_y) // Calling constructor of the super class
	{
		//this->m_cOutput = '@';
		this->m_pRenderer = p_pRenderer; // Set the renderer
		this->m_x = p_x; // X coordinate of player's location
		this->m_y = p_y; // Y coordinate of player's location
		this->m_rSrc = { 0, 0, 107, 138 }; // Set default properties of m_rSrc rect
		this->SetImage(p_pRenderer, p_pTexture); // Set the player image
		updateDst(); // Update player's location on the map
		m_rDst.w = 107; // Set dest rect width
		m_rDst.h = 138; // Set dest rect height
	}
	void setAnimate() { doAnimate = true; }
	void resetAnimate() { doAnimate = false; m_rSrc.x = 0; m_rSrc.y = 0; }
	// Update player's location on the map
	void updateDst() {
		m_rDst.x = m_x;
		m_rDst.y = m_y;
	}
	// Move player by chaning X coordinate
	void MoveX(int p_iX) {
		m_x += p_iX * m_iMoveSpeed;
		updateDst();
	}
	// Move player by changing Y coordinate
	void MoveY(int p_iY) {
		m_y += p_iY * m_iMoveSpeed;
		updateDst();
	}

	void AdvanceAnim()
	{
		if (!this->isPlayerAlive)
			return;
		m_iFrame++; 
		if (m_iFrame == m_iFrameMax) {
			m_iFrame = 0;
			m_iFrameCtr++;
			if (m_iFrameCtr == m_iFrameMax)
			{
				m_iFrameCtr = 0; // Resetting frame counter.
			}
		}
		m_rSrc.x = 107 * m_iFrameCtr; // Prints appropriate 'cell.'
	}
	void SetIdle()
	{
		if (!this->isPlayerAlive)
			return;
		m_iFrameCtr = 0;
		m_rSrc.x = 107 * m_iFrameCtr;
	}

	// Getters for src and dest rect
	const SDL_Rect* GetSrc() { return &m_rSrc; }
	const SDL_Rect* GetDst() { return &m_rDst; }
	void Update() {
		if (doAnimate) {
			if (!isPlayerAlive)
				return;
			m_iFrame++;
			if (m_iFrame == m_iMax) {
				m_iFrame = 0;
				m_iFrameCtr++;
				if (m_iFrameCtr == m_iFrameMax)
				{
					m_iFrameCtr = 0; // Resetting frame counter.
					doAnimate = false;
					AnimFinish = true;
					AnimStart = false;
				}
			}
			m_rSrc.x = m_rSrc.w * m_iFrameCtr; // Prints appropriate 'cell.'
		}
	}

	SDL_Rect getPlayerMidPoint() {
		SDL_Rect midPoint;
		midPoint.x = GetDst()->x + (GetDst()->w / 2);
		midPoint.y = GetDst()->y + (GetDst()->h / 2);
		midPoint.h = 1;
		midPoint.w = 1;
		return midPoint;
	}
};

#endif  