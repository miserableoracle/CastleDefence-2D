/****************************************
Filename:		StateMachine.h
Author:			Prof. Alexander Richard
Modified By:	Hiren Pandya (101198481)
Created On:		09/02/2019
Modified On:	10/03/2019
*****************************************/
#pragma once
#include "pch.h"
#include <iostream>
#include <vector>
#include "Button.h"
#include "Player.h"
#include "SDL_ttf.h"
#include "Arrow.h"
#include "Enemies.h"
#include "SDL_mixer.h"
#define PANELSIZE 4;
using namespace std;

// Abstract base class for all the derived state classes
class State
{
public:
	virtual void Update() = 0;				// Update (pure virtual) function
	virtual void Render();					// Render (virtual) function
	virtual void Enter() = 0;				// State Enter (pure virtual) function
	void RenderFont(bool, const char*, int, int);
	void RenderColorFont(bool, const char*, int, int, SDL_Color);
	virtual void Exit() = 0;				// State Exit (pure virtual) function
	virtual void Resume() = 0;				// State Resume (pure virtual) function
	static bool m_iDebugSet;				// Set variable to print debug traces
	static bool m_iBoxed;					// Set variable to draw boxes on edges for debugging purposes.
	static int m_iSelectedPanel;
	static int m_iGoldCount;
	static vector<int> m_vWeapons;
protected:
	State() {}								// State class default constructor
	TTF_Font* m_Font;
	SDL_Texture* m_pFontText;
	SDL_Rect m_rFontRect;
};

// When game is paused, it will enter PauseState
class PauseState : public State
{
public:
	PauseState() {}							// PauseState class default constructor

	void Update();							// Update function (override State::Update())
	void Render();							// Render function (override State::Render())
	void Enter();							// PauseState Enter function (override State::Enter())
	void Exit();							// PauseState Exit function (override State::Exit())
	void Resume(); 		// PauseState Resume function (override State::Resume())
	enum sfx { hover, click };
	enum btn { resumeGame, shopbtn, exitgame };
private:
	vector<Button*> m_vButtons;
	SDL_Surface* m_pSurf;					// Surface pointer	
	SDL_Texture* m_pText;					// Texture pointer
};

// When user enters the shop
class ShopState : public State {
public:
	ShopState() {}

	void Update();
	void Render();
	void Enter();
	void Exit();
	void Resume() { }
	enum sfx { hover, click };
	enum btn { closeShop, fireArrowBuy, iceArrowBuy, treeRootArrowBuy, fireCannonBuy, iceCarnnonBuy, misslesBuy };

private:
	vector<Button*> m_vButtons;
	SDL_Surface* m_pSurf;
	SDL_Texture* m_pText;

	bool m_bInitialRender = true;
	bool m_bButtonClick = false;
	SDL_Texture* m_pPanel;
	SDL_Texture* m_pFireArrow;
	SDL_Texture* m_pIceArrow;
	SDL_Texture* m_pRootArrow;
	SDL_Texture* m_pFireCannon;
	SDL_Texture* m_pIceCannon;
	SDL_Texture* m_pMissile;
};

// Game state for when game is running
class GameState : public State
{
public:
	GameState() {}							// GameState class default constructor
	void Update();							// Update function (override State::Update())
	void Render();							// Render function (override State::Render())
	void Enter();							// GameState Enter function (override State::Enter())
	void Exit();							// GameState Exit function (override State::Exit())
	void Resume(); 							// GameState Resume function (override State::Resume())
	bool isClickInPanel(int, int, SDL_Rect*);
	enum sfx { arrowcast, arrowhit };
	enum btn { pausebtn, shopbtn };
	Uint8 alpha;
private:
	SDL_Surface* m_pSurf;
	SDL_Texture* m_pTileText;
	SDL_Texture* m_pPlayerText;
	SDL_Texture* m_pBackground;
	SDL_Texture* m_pCastleBack;
	SDL_Texture* m_pCastleFront;
	SDL_Texture* m_pWords;
	SDL_Texture* m_pDefArrowText;
	SDL_Texture* m_pFireArrowText;
	SDL_Texture* m_pIceArrowText;
	SDL_Texture* m_pTreeArrowText;
	SDL_Texture* m_pFireCannonText;
	SDL_Texture* m_pIceCannonText;
	SDL_Texture* m_pMissileText;
	SDL_Texture* m_pEnemyText;
	SDL_Texture* m_pWeaponPanel1;
	SDL_Texture* m_pWeaponPanel2;
	SDL_Texture* m_pWeaponPanel3;
	SDL_Texture* m_pWeaponPanel4;
	SDL_Texture* m_pSelected;
	SDL_Texture* m_pBossEnemy;
	int currLevel = 0;
	Player* m_pPlayer;
	SDL_Color color = { 0, 255, 0 };
	TTF_Font* font;
	int doorStatus = 0;
	time_t startTime = 0;
	time_t endTime = 0;
	time_t swordStartTime = 0;
	time_t swordEndTime = 0;
	int textW, textH;
	SDL_Rect textRect;
	bool m_bOneClick;
	int m_iArrowType = 0;
	int m_iArrowCount = 0;
	int m_iEnemyType = 0;
	vector<Arrow*> m_vArrows;
	vector<Enemy*> m_vEnemies;
	vector<Mix_Chunk*> m_vSounds;
	//int m_iGoldCount = 0;
	string m_sGold;
	int m_iLastGold = -10;
	vector<Button*> m_vButtons;

	SDL_Rect m_rPDst1;
	SDL_Rect m_rPDst2;
	SDL_Rect m_rPDst3;
	SDL_Rect m_rPDst4;
	vector<SDL_Rect*> m_vPanel;
	bool m_bIsPanelUsed[4];
	int m_iWeaponOnPanel[4];
	bool m_bIsWeaponPlaced[4];
};

// Title state class
class TitleState : public State
{
public:
	TitleState();							// TitleState class default constructor
	~TitleState();							// TitleState class destructor
	void Update();							// Update function (override State::Update())
	void Render();							// Render function (override State::Render())
	void Enter();							// TitleState Enter function (override State::Enter())
	void Exit();							// TitleState Exit function (override State::Exit())
	void Resume() {}						// TitleState Resume function (override State::Resume())
	enum sfx { hover, click };
	enum btn { newgame, exitgame };
private:
	vector<Button*> m_vButtons;
	SDL_Surface* m_pSurf;					// Surface pointer	
	SDL_Texture* m_pText;					// Texture pointer
};

// Win state class
class WinState : public State
{
public:
	WinState() {}							// WinState class default constructor
	void Update();							// Update function (override State::Update())
	void Render();							// Render function (override State::Render())
	void Enter();							// WinState Enter function (override State::Enter())
	void Exit();							// WinState Exit function (override State::Exit())
	void Resume() {}						// WinState Resume function (override State::Resume())
	enum sfx { hover, click };
	enum btn { homeScreen, exitgame };
private:
	vector<Button*> m_vButtons;
	SDL_Surface* m_pSurf;					// Surface pointer	
	SDL_Texture* m_pText;					// Texture pointer
	int playOnce = 0;
};

// Lose state class
class LoseState : public State
{
public:
	LoseState() {}							// LoseState class default constructor
	void Update();							// Update function (override State::Update())
	void Render();							// Render function (override State::Render())
	void Enter();							// LoseState Enter function (override State::Enter())
	void Exit();							// LoseState Exit function (override State::Exit())
	void Resume() {}						// LoseState Resume function (override State::Resume())
	enum sfx { hover, click };
	enum btn { homeScreen, exitgame };
private:
	vector<Button*> m_vButtons;
	SDL_Surface* m_pSurf;					// Surface pointer	
	SDL_Texture* m_pText;					// Texture pointer
};

// StateMachine class
class StateMachine
{
public:
	void Update();							// Update function
	void Render();							// Render function
	void PushState(State* pState);			// Add State into the vector
	void ChangeState(State* pState);		// Change from one state to the other
	void PopState();						// Remove state from the vector
	void Clean();							// Clean function
	vector<State*>& GetStates() { return m_vStates; }		// State objects vector getter

private:
	vector<State*> m_vStates;				// State objects vector
};
