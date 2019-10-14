/* File: LOTile.h
Author: Hiren Pandya (101198481)
Created On: 10/03/2019
Modified On: 25/03/2019
*/
#pragma once
#ifndef _LOTILE_H_
#define _LOTILE_H_
#include "Sprite.h" // LOTile inherits Sprite struct publically

// This class will be utilised for our level objects
class LOTile : public Sprite {
public:
	int m_x;
	int m_y;

	LOTile() {
		// Default constructor. Doesn't do anything as of now
	}

	LOTile(int p_x, int p_y) {
		// Parameterized contructor which sets the values of m_x and m_y
		SetX(p_x);
		SetY(p_y);
	}
};

#endif // !_LOTILE_H_
