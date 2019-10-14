/* File: Sprite.h
Author: Hiren Pandya (101198481)
Created On: 10/03/2019
Modified On: 25/03/2019
*/
#pragma once
#ifndef _SPRITE_H_
#define _SPRITE_H_
#include "SDL.h"
// This structure will hold SDL_Rect members 
struct Sprite {
public:
	// For M2, only single variable is required
	char m_cOutput;
	// Now for M3, this class will obtain the much awaited limelight
	SDL_Rect m_rSrc; // Source Rectangle of the sprite
	SDL_Rect m_rDst; // Destination Rectangle of the sprite
	SDL_Texture* m_pTexture; // To draw each image and tile
	SDL_Renderer* m_pRenderer; // To create the texture

	Sprite() { this->m_rSrc = { 0, 0, 32, 32 }; } // Default constructor, set the source to initial tile x = 0, y = 0, h = 32, w = 32
	Sprite(SDL_Rect p_rObjLoc) { this->m_rSrc = p_rObjLoc; }
	~Sprite() { } // Default destructor

	// Update Dest.x coordinate 
	void SetX(const int p_x) {
		this->m_rDst.x = p_x;// *32;
	}
	// Update Dest.y coordinate
	void SetY(const int p_y) {
		this->m_rDst.y = p_y;// *32;
	}
	// Set the image of the tile using texture we created in main
	void SetImage(SDL_Renderer* p_pRenderer, SDL_Texture* p_pTexture) {
		this->m_pRenderer = p_pRenderer;
		this->m_pTexture = p_pTexture;
	}
};
#endif // !_SPRITE_H_
