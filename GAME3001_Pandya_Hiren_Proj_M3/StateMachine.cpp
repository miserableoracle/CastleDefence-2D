/****************************************
Filename:		StateMachine.cpp
Author:			Prof. Alexander Richard
Modified By:	Hiren Pandya (101198481)
Created On:		10/03/2019
Modified On:	25/03/2019
*****************************************/
#include "pch.h"
#include "StateMachine.h"
#include "Game.h"
#include "Sprite.h"
#include "LOTile.h"
#include "Player.h"
#include <time.h>
#include <iostream>
#include <string>
using namespace std;
using namespace chrono;

bool State::m_iDebugSet = false;			// IF Debug traces shall be printed or not;
bool State::m_iBoxed = false;				// If set to true, player sprite edges will be shown as a box
int State::m_iGoldCount = 0;				// Gold count as static variable
vector<int> State::m_vWeapons;
int State::m_iSelectedPanel = 0;

// State Class - Render Function
void State::Render()
{
	SDL_RenderPresent(Game::Instance()->GetRenderer());
}

void State::RenderFont(bool r, const char* c, int x, int y)
{
	if (r)
	{
		SDL_Color textColor = { 255, 255, 255, 0 }; // White text.
		SDL_Surface* fontSurf = TTF_RenderText_Solid(m_Font, c, textColor);
		SDL_DestroyTexture(m_pFontText); // Need to de-allocate previous font texture.
		m_pFontText = SDL_CreateTextureFromSurface(Game::Instance()->GetRenderer(), fontSurf);
		m_rFontRect = { x, y, fontSurf->w, fontSurf->h };
		SDL_FreeSurface(fontSurf);
	}
	SDL_RenderCopy(Game::Instance()->GetRenderer(), m_pFontText, 0, &m_rFontRect);
}

void State::RenderColorFont(bool r, const char* c, int x, int y, SDL_Color textColor)
{
	if (r)
	{
		SDL_Surface* fontSurf = TTF_RenderText_Solid(m_Font, c, textColor);
		SDL_DestroyTexture(m_pFontText); // Need to de-allocate previous font texture.
		m_pFontText = SDL_CreateTextureFromSurface(Game::Instance()->GetRenderer(), fontSurf);
		m_rFontRect = { x, y, fontSurf->w, fontSurf->h };
		SDL_FreeSurface(fontSurf);
	}
	SDL_RenderCopy(Game::Instance()->GetRenderer(), m_pFontText, 0, &m_rFontRect);
}

/*------------- PauseState class -------------*/
// PauseState entered
void PauseState::Enter()
{
	if (State::m_iDebugSet)
		cout << "[PauseState] Game Paused..." << endl;
	
	Game::Instance()->GetAM()->ClearMusic();
	Game::Instance()->GetAM()->LoadMusic("Assets/sounds/menu.mp3");
	Game::Instance()->GetAM()->PlayMusic();

	m_vButtons.push_back(new Button("Assets/imgs/buttons/resume.png", { (1024 / 2) - 200, (768 / 2), 350, 46 }, "Assets/sounds/btnHover.wav", "Assets/sounds/btnClick.wav"));
	m_vButtons.push_back(new Button("Assets/imgs/buttons/shop.png", { (1024 / 2) - 200, (768 / 2) + 50, 350, 46 }, "Assets/sounds/btnHover.wav", "Assets/sounds/btnClick.wav"));
	m_vButtons.push_back(new Button("Assets/imgs/buttons/exitGame.png", { (1024 / 2) - 200, (768 / 2) + 100, 350, 46 }, "Assets/sounds/btnHover.wav", "Assets/sounds/btnClick.wav"));

	m_pSurf = IMG_Load("Assets/imgs/title.jpg");
	m_pText = SDL_CreateTextureFromSurface(Game::Instance()->GetRenderer(), m_pSurf);
	SDL_FreeSurface(m_pSurf);

}

// PauseState Update 
void PauseState::Update()
{
	if (Game::Instance()->KeyDown(SDL_SCANCODE_R) == 1) {
		Game::Instance()->GetFSM()->PopState();
		return;
	}
	if (State::m_iDebugSet)
		cout << "[PauseState] Rendering PauseState..." << endl;

	if (m_vButtons[btn::resumeGame]->Clicked()) {
		SDL_Delay(100);
		Game::Instance()->GetFSM()->PopState();
		return;
	}
	else if (m_vButtons[btn::shopbtn]->Clicked()) {
		SDL_Delay(100);
		Game::Instance()->GetFSM()->PushState(new ShopState());
		m_vButtons[btn::shopbtn]->ReleaseClick();
		return;
	}
	else if (m_vButtons[btn::exitgame]->Clicked())
	{
		SDL_Delay(500); // Just pause to let the button sound play.
		Game::Instance()->DoQuit();
		return;
	}

	for (int i = 0; i < m_vButtons.size(); i++)
	{
		m_vButtons[i]->Update();
	}

}

// PauseState Render
void PauseState::Render()
{
	if (State::m_iDebugSet)
		cout << "[PauseState] Rendering Pause Screen..." << endl;
	
	SDL_Rect m_rSrc = { 0,0,1024,768 };
	SDL_Rect m_rDst = { 0,0,1024,768 };
	SDL_RenderCopy(Game::Instance()->GetRenderer(), m_pText, &m_rSrc, &m_rDst);
	
	for (int i = 0; i < m_vButtons.size(); i++)
	{
		m_vButtons[i]->Render();
	}
	State::Render();
}

// Exit the PauseState
void PauseState::Exit()
{
	if (State::m_iDebugSet)
		cout << "[PauseState] Resuming the Game..." << endl;
	SDL_DestroyTexture(m_pText);
	Game::Instance()->GetAM()->ClearMusic();
	for (int i = 0; i < m_vButtons.size(); i++)
	{
		delete m_vButtons[i];
		m_vButtons.erase(m_vButtons.begin() + i);
	}
}

void PauseState::Resume() {
	Game::Instance()->GetAM()->ClearMusic();
	Game::Instance()->GetAM()->LoadMusic("Assets/sounds/menu.mp3");
	Game::Instance()->GetAM()->PlayMusic();
}
/*------------- WinState class -------------*/
// WinState entered
void WinState::Enter()
{
	if (State::m_iDebugSet)
		cout << "[WinState] Winner..." << endl;
}

// WinState Update 
void WinState::Update()
{
	if (State::m_iDebugSet)
		cout << "[WinState] Rendering WinState..." << endl;
}

// WinState Render
void WinState::Render()
{
	if (State::m_iDebugSet)
		cout << "[WinState] Rendering WinState..." << endl;
	SDL_RenderClear(Game::Instance()->GetRenderer());

	State::Render();
}

// Exit the WinState
void WinState::Exit()
{
	if (State::m_iDebugSet)
		cout << "[WinState] Exiting the Win State" << endl;
}

/*------------- LoseState class -------------*/
// LoseState entered
void LoseState::Enter()
{
	if (State::m_iDebugSet)
		cout << "[LoseState] Game Over..." << endl;
}

// LoseState Update 
void LoseState::Update()
{
	
}

// LoseState Render
void LoseState::Render()
{
	if (State::m_iDebugSet)
		cout << "[LoseState] Rendering WinState..." << endl;
	State::Render();
}

// Exit the LoseState
void LoseState::Exit()
{
	cout << "[LoseState] Exiting the Lose State" << endl;
	
}

/*------------- GameState class -------------*/
// GameState Entered
void GameState::Enter()
{
	if (State::m_iDebugSet)
		cout << "[GameState] Game State Entered..." << endl;

	for (int i = 0; i < 4; i++)
	{
		m_bIsPanelUsed[i] = false;
	}
	// Load font
	m_Font = TTF_OpenFont("Assets/imgs/LTYPE.TTF", 20);
	// Load Game Music
	Game::Instance()->GetAM()->LoadMusic("Assets/sounds/game.mp3");
	Game::Instance()->GetAM()->PlayMusic();
	m_vSounds.push_back(Mix_LoadWAV("Assets/sounds/arrowshoot.wav"));
	m_vSounds.push_back(Mix_LoadWAV("Assets/sounds/arrowhit.wav"));
	// Load background 
	m_pSurf = IMG_Load("Assets/imgs/bgd.jpg");
	m_pBackground = SDL_CreateTextureFromSurface(Game::Instance()->GetRenderer(), m_pSurf);
	SDL_FreeSurface(m_pSurf);

	m_pSurf = IMG_Load("Assets/imgs/castle.png");
	m_pCastleBack = SDL_CreateTextureFromSurface(Game::Instance()->GetRenderer(), m_pSurf);
	SDL_FreeSurface(m_pSurf);

	m_pSurf = IMG_Load("Assets/imgs/castle.png");
	m_pCastleFront = SDL_CreateTextureFromSurface(Game::Instance()->GetRenderer(), m_pSurf);
	SDL_FreeSurface(m_pSurf);

	m_pSurf = IMG_Load("Assets/imgs/gaurdSprite.png");
	m_pPlayerText = SDL_CreateTextureFromSurface(Game::Instance()->GetRenderer(), m_pSurf);
	SDL_FreeSurface(m_pSurf);

	m_pSurf = IMG_Load("Assets/imgs/Arrow.png");
	m_pDefArrowText = SDL_CreateTextureFromSurface(Game::Instance()->GetRenderer(), m_pSurf);
	SDL_FreeSurface(m_pSurf);

	m_pSurf = IMG_Load("Assets/imgs/firearrow.png");
	m_pFireArrowText = SDL_CreateTextureFromSurface(Game::Instance()->GetRenderer(), m_pSurf);
	SDL_FreeSurface(m_pSurf);

	m_pSurf = IMG_Load("Assets/imgs/icearrow.png");
	m_pIceArrowText = SDL_CreateTextureFromSurface(Game::Instance()->GetRenderer(), m_pSurf);
	SDL_FreeSurface(m_pSurf);

	m_pSurf = IMG_Load("Assets/imgs/treerootarrow.png");
	m_pTreeArrowText = SDL_CreateTextureFromSurface(Game::Instance()->GetRenderer(), m_pSurf);
	SDL_FreeSurface(m_pSurf);

	m_pSurf = IMG_Load("Assets/imgs/firecannon.png");
	m_pFireCannonText = SDL_CreateTextureFromSurface(Game::Instance()->GetRenderer(), m_pSurf);
	SDL_FreeSurface(m_pSurf);

	m_pSurf = IMG_Load("Assets/imgs/icecannon.png");
	m_pIceCannonText = SDL_CreateTextureFromSurface(Game::Instance()->GetRenderer(), m_pSurf);
	SDL_FreeSurface(m_pSurf);

	m_pSurf = IMG_Load("Assets/imgs/missile.png");
	m_pMissileText = SDL_CreateTextureFromSurface(Game::Instance()->GetRenderer(), m_pSurf);
	SDL_FreeSurface(m_pSurf);

	m_pSurf = IMG_Load("Assets/imgs/goblin.png");
	m_pEnemyText = SDL_CreateTextureFromSurface(Game::Instance()->GetRenderer(), m_pSurf);
	SDL_FreeSurface(m_pSurf);

	m_pSurf = IMG_Load("Assets/imgs/selected.png");
	m_pSelected = SDL_CreateTextureFromSurface(Game::Instance()->GetRenderer(), m_pSurf);
	SDL_FreeSurface(m_pSurf);

	m_pSurf = IMG_Load("Assets/imgs/boss.png");
	m_pBossEnemy = SDL_CreateTextureFromSurface(Game::Instance()->GetRenderer(), m_pSurf);
	SDL_FreeSurface(m_pSurf);

	m_pSurf = IMG_Load("Assets/imgs/panel.png");
	m_pWeaponPanel1 = SDL_CreateTextureFromSurface(Game::Instance()->GetRenderer(), m_pSurf);
	m_pWeaponPanel2 = SDL_CreateTextureFromSurface(Game::Instance()->GetRenderer(), m_pSurf);
	m_pWeaponPanel3 = SDL_CreateTextureFromSurface(Game::Instance()->GetRenderer(), m_pSurf);
	m_pWeaponPanel4 = SDL_CreateTextureFromSurface(Game::Instance()->GetRenderer(), m_pSurf);
	SDL_FreeSurface(m_pSurf);

	m_vButtons.push_back(new Button("Assets/imgs/buttons/gamepausebtn.png", { 1024 - 200, 20, 80, 36 }, "Assets/sounds/btnHover.wav", "Assets/sounds/btnClick.wav"));
	m_vButtons.push_back(new Button("Assets/imgs/buttons/gameshopbtn.png", { 1024 - 100, 20, 80, 36 }, "Assets/sounds/btnHover.wav", "Assets/sounds/btnClick.wav"));
	
	// Reset Arrow Type to default
	this->m_iArrowType = 0;
	this->m_bOneClick = false;
	m_pPlayer = new Player(130, 500, Game::Instance()->GetRenderer(), m_pPlayerText);
	m_vEnemies.push_back(new Enemy(1224, 650, 1, 200, 650, Game::Instance()->GetRenderer(), m_pEnemyText));

	// Default weapon
	State::m_vWeapons.push_back(0);
	m_bIsPanelUsed[0] = true;
	m_iWeaponOnPanel[0] = 0;
	State::m_iSelectedPanel = 0;
}

// GameState Update function
void GameState::Update()
{
	if (Game::Instance()->KeyDown(SDL_SCANCODE_P) == 1) {				// Pause the game
		Game::Instance()->GetFSM()->PushState(new PauseState());
		return;
	}
	if (Game::Instance()->KeyDown(SDL_SCANCODE_Q) == 1) {				// Pause the game
		State::m_iGoldCount += 100;
		return;
	}
	else if (Game::Instance()->KeyDown(SDL_SCANCODE_X) == 1) {		// Back to Title Screen
		Game::Instance()->GetFSM()->ChangeState(new TitleState());
		return;
	}

	if (m_vEnemies.size() < 5) {
		m_vEnemies.push_back(new Enemy(1224 + ((rand() % 200) + 100), 650, 1, 200, 650, Game::Instance()->GetRenderer(), m_pEnemyText));
	}

	if (m_vEnemies.size() < 6 && State::m_iGoldCount % 1500 == 0 && State::m_iGoldCount != 0) {
		m_vEnemies.push_back(new Enemy(1224 + ((rand() % 300) + 100), 650, 2, 400, 650, Game::Instance()->GetRenderer(), m_pBossEnemy));
	}

	if (Game::Instance()->GetLeftMouse()) {
		int x = 0, y = 0;
		if (!m_bOneClick) {
			SDL_GetMouseState(&x, &y);
		}
		//if (this->m_iArrowType == 0 || this->m_iArrowType == 1 || this->m_iArrowType == 2 || this->m_iArrowType == 3) {
			m_pPlayer->setAnimate();
			m_pPlayer->AnimStart = true;
			m_pPlayer->AnimFinish = false;
			m_bOneClick = true;
		//}

		for (int i = 0; i < 4; i++)
		{
			if (isClickInPanel(x, y, m_vPanel[i])) {
				cout << "Clicked Panel - " << m_vPanel[i]->x << " " << m_vPanel[i]->y << " Arrow Type - " << m_iArrowType << endl;
				if (m_bIsPanelUsed[i]) {
					this->m_iArrowType = m_iWeaponOnPanel[i];
					State::m_iSelectedPanel = i;

					//if(m_iWeaponOnPanel[i] == 1)
				}
			}
		}
	}

	if (!Game::Instance()->GetLeftMouse() && m_bOneClick) {
		if (m_pPlayer->AnimFinish) {
			if (m_iArrowType == 0) {
				int x = 0;
				int y = 0;
				SDL_GetMouseState(&x, &y);
				Arrow* arrow = new Arrow(m_pPlayer->getPlayerMidPoint().x - 20, m_pPlayer->getPlayerMidPoint().y - 20, x, y, Game::Instance()->GetRenderer(), m_pDefArrowText);
				arrow->m_dAngle = atan2(y - m_pPlayer->getPlayerMidPoint().y, x - m_pPlayer->getPlayerMidPoint().x);
				arrow->m_Dx = cos(arrow->m_dAngle);
				arrow->m_Dy = sin(arrow->m_dAngle);
				m_vArrows.push_back(arrow);
				//Game::Instance()->GetAM()->PlaySound(0);
				Mix_PlayChannel(-1, m_vSounds[0], 0);
				if (State::m_iDebugSet)
					cout << "Mouse X: " << x << " Mouse Y: " << y << endl;
				m_bOneClick = false;
			}
			else if (m_iArrowType == 1) {
				m_bOneClick = false;
			}
			else if (m_iArrowType == 2) {
				m_bOneClick = false;
			}
			else if (m_iArrowType == 3) {
				m_bOneClick = false;
			}
		}
	}

	for (int i = 0; i < m_vArrows.size(); i++)
	{
		m_vArrows[i]->Update();
		// Delete arrow when out of bound
		for (int j = 0; j < m_vEnemies.size(); j++)
		{
			if (SDL_HasIntersection(m_vArrows[i]->GetDst(), &(m_vEnemies[j]->bounds)) && !m_vArrows[i]->m_bHitEnemy && !m_vEnemies[j]->isHit) {
				m_vArrows[i]->m_bHitEnemy = true;
				m_vEnemies[j]->isHit = true;
				//Game::Instance()->GetAM()->PlaySound(1);
				Mix_PlayChannel(-1, m_vSounds[1], 0);
			}
		}

		if (m_vArrows[i]->m_bOutOfBounds || !m_vArrows[i]->isActive) {
			delete m_vArrows[i];
			m_vArrows.erase(m_vArrows.begin() + i);
		}

	}
	m_pPlayer->Update();

	for (int i = 0; i < m_vEnemies.size(); i++)
	{
		if (!m_vEnemies[i]->isHit) {
			m_vEnemies[i]->Update();
			m_vEnemies[i]->MoveX(-1);
		}
		if (!m_vEnemies[i]->isAlive) {
			delete m_vEnemies[i];
			m_vEnemies.erase(m_vEnemies.begin() + i);
		}
	}

	if (m_vButtons[btn::shopbtn]->Clicked())
	{
		SDL_Delay(100); // Just pause to let the button sound play.
		Game::Instance()->GetFSM()->PushState(new ShopState());
		m_vButtons[btn::shopbtn]->ReleaseClick();
		return;
	}
	else if (m_vButtons[btn::pausebtn]->Clicked()) {
		SDL_Delay(100); // Just pause to let the button sound play.
		Game::Instance()->GetFSM()->PushState(new PauseState());
		m_vButtons[btn::pausebtn]->ReleaseClick();
		return;
	}

	for (int i = 0; i < m_vButtons.size(); i++)
	{
		m_vButtons[i]->Update();
	}
}

// GameState Render
void GameState::Render()
{
	if (State::m_iDebugSet)
		cout << "[GameState] Rendering Game..." << endl;
	//SDL_SetRenderDrawColor(Game::Instance()->GetRenderer(), 0, 255, 0, 255);
	SDL_RenderClear(Game::Instance()->GetRenderer());

	// Render background
	SDL_Rect m_rSrc = { 0,0,1024,768 };
	SDL_Rect m_rDst = { 0,0,1024,768 };
	SDL_RenderCopy(Game::Instance()->GetRenderer(), m_pBackground, &m_rSrc, &m_rDst);
	// Render castle_back
	m_rDst = { -250, 80, 600, 707 };
	SDL_RenderCopy(Game::Instance()->GetRenderer(), m_pCastleBack, &m_rSrc, &m_rDst);
	// Render castle front
	m_rDst = { -150, 70, 600, 707 };
	SDL_RenderCopy(Game::Instance()->GetRenderer(), m_pCastleFront, &m_rSrc, &m_rDst);
	// Render player
	SDL_RenderCopy(Game::Instance()->GetRenderer(), m_pPlayerText, m_pPlayer->GetSrc(), m_pPlayer->GetDst());
	// Render arrows along with their type
	for (int i = 0; i < m_vArrows.size(); i++)
	{
		if (m_vArrows[i]->m_iBlinkCounter == 3 && m_vArrows[i]->m_bHitEnemy) {
			State::m_iGoldCount += 100;
			m_vArrows[i]->isActive = false;
			m_vArrows[i]->m_iBlinkCounter = 0;
		}

		if (m_vArrows[i]->m_bHitEnemy && m_vArrows[i]->isActive) {
			m_vArrows[i]->m_iACount++;
			if (m_vArrows[i]->m_iACount >= 18) {
				if (this->m_iArrowType == 0)
					SDL_RenderCopyEx(Game::Instance()->GetRenderer(), m_pDefArrowText, m_vArrows[i]->GetSrc(), m_vArrows[i]->GetDst(), m_vArrows[i]->m_dAngle * (180 / M_PI), 0, SDL_FLIP_NONE);
				if (m_vArrows[i]->m_iACount == 40) {
					m_vArrows[i]->m_iACount = 0;
					m_vArrows[i]->m_iBlinkCounter++;
				}	
			}
		}
		else {
			if (this->m_iArrowType == 0 && m_vArrows[i]->isActive)
				SDL_RenderCopyEx(Game::Instance()->GetRenderer(), m_pDefArrowText, m_vArrows[i]->GetSrc(), m_vArrows[i]->GetDst(), m_vArrows[i]->m_dAngle * (180 / M_PI), 0, SDL_FLIP_NONE);
		}
	}

	for (int i = 0; i < m_vEnemies.size(); i++)
	{
		if (m_vEnemies[i]->m_iBlinkCounter == 3 && m_vEnemies[i]->isHit) {
			m_vEnemies[i]->isAlive = false;
		}

		if (m_vEnemies[i]->isHit && m_vEnemies[i]->isAlive) {
			m_vEnemies[i]->m_iECount++;
			if (m_vEnemies[i]->m_iECount >= 18) {
				if (m_vEnemies[i]->m_iEnemyType == 1)
					SDL_RenderCopyEx(Game::Instance()->GetRenderer(), m_pEnemyText, m_vEnemies[i]->GetSrc(), m_vEnemies[i]->GetDst(), 0, 0, SDL_FLIP_NONE);
				if (m_vEnemies[i]->m_iEnemyType == 2)
					SDL_RenderCopyEx(Game::Instance()->GetRenderer(), m_pBossEnemy, m_vEnemies[i]->GetSrc(), m_vEnemies[i]->GetDst(), 0, 0, SDL_FLIP_NONE);
				if (m_vEnemies[i]->m_iECount == 40) {
					m_vEnemies[i]->m_iECount = 0;
					m_vEnemies[i]->m_iBlinkCounter++;
				}
			}
		}
		else {
			if (m_vEnemies[i]->m_iEnemyType == 1 && m_vEnemies[i]->isAlive) {
				SDL_RenderCopyEx(Game::Instance()->GetRenderer(), m_pEnemyText, m_vEnemies[i]->GetSrc(), m_vEnemies[i]->GetDst(), 0, 0, SDL_FLIP_NONE);
				m_vEnemies[i]->bounds.x = m_vEnemies[i]->m_x + 30;
				m_vEnemies[i]->bounds.y = m_vEnemies[i]->m_y + 20;
				m_vEnemies[i]->bounds.w = (m_vEnemies[i]->m_rDst.w / 2) - 10;
				m_vEnemies[i]->bounds.h = (m_vEnemies[i]->m_rDst.h / 2) + 10;
			}

			if (m_vEnemies[i]->m_iEnemyType == 2 && m_vEnemies[i]->isAlive) {
				SDL_RenderCopyEx(Game::Instance()->GetRenderer(), m_pBossEnemy, m_vEnemies[i]->GetSrc(), m_vEnemies[i]->GetDst(), 0, 0, SDL_FLIP_NONE);
			}
		}
	}

	if (State::m_iBoxed) {
		SDL_SetRenderDrawColor(Game::Instance()->GetRenderer(), 0x00, 0x00, 0xFF, 0x00);
		SDL_Rect* pPosition = new SDL_Rect();
		pPosition->x = m_pPlayer->getPlayerMidPoint().x;
		pPosition->y = m_pPlayer->getPlayerMidPoint().y;
		pPosition->w = 50;
		pPosition->h = 50;
		SDL_RenderDrawRect(Game::Instance()->GetRenderer(), pPosition);
		for (int i = 0; i < m_vEnemies.size(); i++)
		{
			SDL_RenderDrawRect(Game::Instance()->GetRenderer(), &(m_vEnemies[i]->bounds));
		}
	}
	m_sGold = "Gold: " + to_string(State::m_iGoldCount);
	RenderFont((m_iLastGold < State::m_iGoldCount) ? 1 : 0, m_sGold.c_str(), 30, 30);
	m_iLastGold = State::m_iGoldCount;
	//if(m_iLastGold < State::m_iGoldCount) 
	for (int i = 0; i < m_vButtons.size(); i++)
	{
		m_vButtons[i]->Render();
	}

	// Render weapon's panel
	SDL_Rect m_rPSrc = { 0, 0, 100, 100 };
	m_rPDst1 = { 300, 20, 100, 100 };
	SDL_RenderCopy(Game::Instance()->GetRenderer(), m_pWeaponPanel1, &m_rPSrc, &m_rPDst1);

	m_rPDst2 = { m_rPDst1.x + 120, 20, 100, 100 };
	SDL_RenderCopy(Game::Instance()->GetRenderer(), m_pWeaponPanel2, &m_rPSrc, &m_rPDst2);
	m_rPDst3 = { m_rPDst2.x + 120, 20, 100, 100 };
	SDL_RenderCopy(Game::Instance()->GetRenderer(), m_pWeaponPanel3, &m_rPSrc, &m_rPDst3);
	m_rPDst4 = { m_rPDst3.x + 120, 20, 100, 100 };
	SDL_RenderCopy(Game::Instance()->GetRenderer(), m_pWeaponPanel4, &m_rPSrc, &m_rPDst4);

	SDL_Rect m_rSelSrc = { 0, 0, 24, 24 };
	SDL_Rect m_rSelDst = { 300 + (State::m_iSelectedPanel * 120) + 70, 90, 24, 24 };
	SDL_RenderCopy(Game::Instance()->GetRenderer(), m_pSelected, &m_rSelSrc, &m_rSelDst);

	SDL_Rect m_rArrowSrc = { 0, 0, 68, 14 };
	SDL_Rect m_rArrowDst = { m_rPDst1.x + 10, m_rPDst1.y + 50, 68, 14 };
	SDL_RenderCopyEx(Game::Instance()->GetRenderer(), m_pDefArrowText, &m_rArrowSrc, &m_rArrowDst, -45, NULL, SDL_FLIP_NONE);

	bool m_bWeaponRender;
	SDL_Rect m_rFireArrowsSrc = { 0, 0, 32, 90 };
	SDL_Rect m_rCannonsSrc = { 0, 0, 89, 90 };
	SDL_Rect m_rMissileSrc = { 0, 0, 80, 75 };
	for (int i = 0; i < State::m_vWeapons.size(); i++)
	{
		bool isAlreadySet = false;
		if (State::m_vWeapons[i] == 1) {
			for (int j = 1; j < 4; j++)
			{
				if (!m_bIsPanelUsed[j] && !isAlreadySet && !m_bIsWeaponPlaced[i]) {
					m_bIsPanelUsed[j] = true;
					isAlreadySet = true;
					m_iWeaponOnPanel[j] = 1;
					m_bIsWeaponPlaced[i] = true;
					break;
				}
				if (m_bIsPanelUsed[j] && m_iWeaponOnPanel[j] == 1) {
					SDL_Rect m_rArrowDst = { 300 + (j * 120) + 30, 20, 32, 90 };
					SDL_RenderCopyEx(Game::Instance()->GetRenderer(), m_pFireArrowText, &m_rFireArrowsSrc, &m_rArrowDst, 45, NULL, SDL_FLIP_NONE);
				}
			}
		}
		else if (State::m_vWeapons[i] == 2) {
			for (int j = 1; j < 4; j++)
			{
				if (!m_bIsPanelUsed[j] && !isAlreadySet && !m_bIsWeaponPlaced[i]) {
					m_bIsPanelUsed[j] = true;
					isAlreadySet = true;
					m_iWeaponOnPanel[j] = 2;
					m_bIsWeaponPlaced[i] = true;
					break;
				}
				if (m_bIsPanelUsed[j] && m_iWeaponOnPanel[j] == 2) {
					SDL_Rect m_rArrowDst = { 300 + (j * 120) + 30, 20, 32, 90 };
					SDL_RenderCopyEx(Game::Instance()->GetRenderer(), m_pIceArrowText, &m_rFireArrowsSrc, &m_rArrowDst, 45, NULL, SDL_FLIP_NONE);
				}
			}
		}
		else if (State::m_vWeapons[i] == 3) {
			for (int j = 1; j < 4; j++)
			{
				if (!m_bIsPanelUsed[j] && !isAlreadySet && !m_bIsWeaponPlaced[i]) {
					m_bIsPanelUsed[j] = true;
					isAlreadySet = true;
					m_iWeaponOnPanel[j] = 3;
					m_bIsWeaponPlaced[i] = true;
					break;
				}
				if (m_bIsPanelUsed[j] && m_iWeaponOnPanel[j] == 3) {
					SDL_Rect m_rArrowDst = { 300 + (j * 120) + 30, 20, 32, 90 };
					SDL_RenderCopyEx(Game::Instance()->GetRenderer(), m_pTreeArrowText, &m_rFireArrowsSrc, &m_rArrowDst, 45, NULL, SDL_FLIP_NONE);
				}
			}
		}
		else if (State::m_vWeapons[i] == 4) {
			for (int j = 1; j < 4; j++)
			{
				if (!m_bIsPanelUsed[j] && !isAlreadySet && !m_bIsWeaponPlaced[i]) {
					m_bIsPanelUsed[j] = true;
					isAlreadySet = true;
					m_iWeaponOnPanel[j] = 4;
					m_bIsWeaponPlaced[i] = true;
					break;
				}
				if (m_bIsPanelUsed[j] && m_iWeaponOnPanel[j] == 4) {
					SDL_Rect m_rCannonDst = { 300 + (j * 120) + 10, 20, 89, 90 };
					SDL_RenderCopyEx(Game::Instance()->GetRenderer(), m_pFireCannonText, &m_rCannonsSrc, &m_rCannonDst, 0, NULL, SDL_FLIP_NONE);
				}
			}
		}
		else if (State::m_vWeapons[i] == 5) {
			for (int j = 1; j < 4; j++)
			{
				if (!m_bIsPanelUsed[j] && !isAlreadySet && !m_bIsWeaponPlaced[i]) {
					m_bIsPanelUsed[j] = true;
					isAlreadySet = true;
					m_iWeaponOnPanel[j] = 5;
					m_bIsWeaponPlaced[i] = true;
					break;
				}
				if (m_bIsPanelUsed[j] && m_iWeaponOnPanel[j] == 5) {
					SDL_Rect m_rCannonDst = { 300 + (j * 120) + 10, 20, 89, 90 };
					SDL_RenderCopyEx(Game::Instance()->GetRenderer(), m_pIceCannonText, &m_rCannonsSrc, &m_rCannonDst, 0, NULL, SDL_FLIP_NONE);
				}
			}
		}
		else if (State::m_vWeapons[i] == 6) {
			for (int j = 1; j < 4; j++)
			{
				if (!m_bIsPanelUsed[j] && !isAlreadySet && !m_bIsWeaponPlaced[i]) {
					m_bIsPanelUsed[j] = true;
					isAlreadySet = true;
					m_iWeaponOnPanel[j] = 6;
					m_bIsWeaponPlaced[i] = true;
					break;
				}
				if (m_bIsPanelUsed[j] && m_iWeaponOnPanel[j] == 6) {
					SDL_Rect m_rMissileDst = { 300 + (j * 120) + 10, 35, 80, 75 };
					SDL_RenderCopyEx(Game::Instance()->GetRenderer(), m_pMissileText, &m_rMissileSrc, &m_rMissileDst, 0, NULL, SDL_FLIP_NONE);
				}
			}
		}
	}

	if (m_vPanel.size() == 0) {
		m_vPanel.push_back(&m_rPDst1);
		m_vPanel.push_back(&m_rPDst2);
		m_vPanel.push_back(&m_rPDst3);
		m_vPanel.push_back(&m_rPDst4);
	}
	State::Render();
}

void GameState::Resume() {
	if (State::m_iDebugSet) cout << "Resuming Game..." << endl;
	Game::Instance()->GetAM()->LoadMusic("Assets/sounds/game.mp3");
	Game::Instance()->GetAM()->PlayMusic();
}

bool GameState::isClickInPanel(int x, int y, struct SDL_Rect *rect)
{
	if (x >= rect->x && x < (rect->x + rect->w))
	{
		if (y >= rect->y && y < (rect->y + rect->h))
		{
			return 1;
		}
	}
	return 0;
}

// GameState exit
void GameState::Exit()
{
	cout << "[GameState] Exiting Game..." << endl;
	Game::Instance()->GetAM()->ClearMusic();
	Game::Instance()->GetAM()->ClearSounds();

	for (int i = 0; i < (int)m_vSounds.size(); i++)
	{
		Mix_FreeChunk(m_vSounds[i]);
		m_vSounds[i] = nullptr;
	}
	m_vSounds.clear();

	TTF_CloseFont(m_Font);
	SDL_DestroyTexture(m_pTileText);
	SDL_DestroyTexture(m_pPlayerText);
	SDL_DestroyTexture(m_pBackground);
	SDL_DestroyTexture(m_pCastleBack);
	SDL_DestroyTexture(m_pCastleFront);
	SDL_DestroyTexture(m_pWords);
	SDL_DestroyTexture(m_pDefArrowText);
	SDL_DestroyTexture(m_pFireArrowText);
	SDL_DestroyTexture(m_pIceArrowText);
	SDL_DestroyTexture(m_pTreeArrowText);
	SDL_DestroyTexture(m_pFireCannonText);
	SDL_DestroyTexture(m_pIceCannonText);
	SDL_DestroyTexture(m_pMissileText);
	SDL_DestroyTexture(m_pEnemyText);
	SDL_DestroyTexture(m_pWeaponPanel1);
	SDL_DestroyTexture(m_pWeaponPanel2);
	SDL_DestroyTexture(m_pWeaponPanel3);
	SDL_DestroyTexture(m_pWeaponPanel4);
	SDL_DestroyTexture(m_pSelected);
	delete m_pPlayer;
}

/*------------- TitleState class -------------*/
// Default constructor
TitleState::TitleState() {

}

// Destructor
TitleState::~TitleState() {

}

// TitleState enter
void TitleState::Enter()
{
	if (State::m_iDebugSet)
		cout << "[TitleState] Entering Title..." << endl;
	m_vButtons.push_back(new Button("Assets/imgs/buttons/newGame.png", { (1024 / 2) + 100, (768 / 2) + 200, 350, 46 }, "Assets/sounds/btnHover.wav", "Assets/sounds/btnClick.wav"));
	m_vButtons.push_back(new Button("Assets/imgs/buttons/exitGame.png", { (1024 / 2) + 100, (768 / 2) + 250, 350, 46 }, "Assets/sounds/btnHover.wav", "Assets/sounds/btnClick.wav"));
	
	// To disasble any button
	//m_vButtons[1]->SetButtonState(true);
	Game::Instance()->GetAM()->LoadMusic("Assets/sounds/menu.mp3");
	Game::Instance()->GetAM()->PlayMusic();

	m_pSurf = IMG_Load("Assets/imgs/title.jpg");
	m_pText = SDL_CreateTextureFromSurface(Game::Instance()->GetRenderer(), m_pSurf);
	SDL_FreeSurface(m_pSurf);
}

// TitleState Update function
void TitleState::Update()
{
	if (Game::Instance()->KeyDown(SDL_SCANCODE_N) == 1) {			// New Game 
		Game::Instance()->GetFSM()->ChangeState(new GameState());
		return;
	}
	
	if (m_vButtons[btn::newgame]->Clicked()) {
		Game::Instance()->GetFSM()->ChangeState(new GameState());
		return;
	}

	else if (m_vButtons[btn::exitgame]->Clicked())
	{
		SDL_Delay(500); // Just pause to let the button sound play.
		Game::Instance()->DoQuit();
		return;
	}

	for (int i = 0; i < m_vButtons.size(); i++)
	{
		m_vButtons[i]->Update();
	}
}

// TitleState Render
void TitleState::Render()
{
	if (State::m_iDebugSet)
		cout << "[TitleState] Rendering Title..." << endl;
	
	SDL_Rect m_rSrc = { 0,0,1024,768 };
	SDL_Rect m_rDst = { 0,0,1024,768 };
	SDL_RenderCopy(Game::Instance()->GetRenderer(), m_pText, &m_rSrc, &m_rDst);

	for (int i = 0; i < m_vButtons.size(); i++)
	{
		m_vButtons[i]->Render();
	}

	State::Render();
}

// TitleState Exit
void TitleState::Exit()
{
	cout << "[TitleState] Exiting Title..." << endl;
	for (int i = 0; i < (int)m_vButtons.size(); i++)
	{
		delete m_vButtons[i];
		m_vButtons[i] = nullptr;
		m_vButtons.erase(m_vButtons.begin() + i);
	}
	m_vButtons.clear();
	m_vButtons.shrink_to_fit();
	Game::Instance()->GetAM()->ClearSounds();
	Game::Instance()->GetAM()->ClearMusic();
}

/*------------- StateMachine class -------------*/
// StateMachine Update function
void StateMachine::Update()
{
	if (!m_vStates.empty()) // empty() and back() are methods of the vector type.
		m_vStates.back()->Update();
}

// StateMachine Render
void StateMachine::Render()
{
	if (!m_vStates.empty())
		m_vStates.back()->Render();
}

// Push new state into state vector
void StateMachine::PushState(State* pState)
{
	m_vStates.push_back(pState); // push_back() is a method of the vector type.
	m_vStates.back()->Enter();
}

// Change state
void StateMachine::ChangeState(State* pState)
{
	if (!m_vStates.empty())
	{
		m_vStates.back()->Exit();
		delete m_vStates.back(); // De-allocating the state in the heap.
		m_vStates.back() = nullptr; // Nullifying pointer to the de-allocated state.
		m_vStates.pop_back(); // Removes the now-null pointer from the vector.
	}
	pState->Enter();
	m_vStates.push_back(pState);
}

// Remove the state from the state vector
void StateMachine::PopState()
{
	if (!m_vStates.empty())
	{
		m_vStates.back()->Exit();
		delete m_vStates.back();
		m_vStates.back() = nullptr;
		m_vStates.pop_back();
	}
	m_vStates.back()->Resume();
}

// Clear the StateMachine
void StateMachine::Clean()
{
	while (!m_vStates.empty()) // Because we can exit the game in the pause state with the window's 'X'.
	{						   // Ensures that ALL states left in the vector are cleaned up.
		m_vStates.back()->Exit();
		delete m_vStates.back();
		m_vStates.back() = nullptr;
		m_vStates.pop_back();
	}
}

void ShopState::Update()
{
	if (Game::Instance()->KeyDown(SDL_SCANCODE_R) == 1) {
		Game::Instance()->GetFSM()->PopState();
		return;
	}
	if (State::m_iDebugSet)
		cout << "[ShopState] Rendering Shop..." << endl;

	if (m_vButtons[btn::closeShop]->Clicked()) {
		SDL_Delay(100);
		Game::Instance()->GetFSM()->PopState();
		return;
	}
	
	for (int i = 0; i < m_vButtons.size(); i++)
	{
		m_vButtons[i]->Update();
	}

	if (m_bButtonClick) {
		if (!Game::Instance()->GetLeftMouse())
			m_bButtonClick = false;
	}

	// Update buttons based on gold level
	if (State::m_iGoldCount < 300) {
		m_vButtons[btn::fireArrowBuy]->SetButtonState(true);
	}
	else {
		m_vButtons[btn::fireArrowBuy]->SetButtonState(false);
	}

	if (State::m_iGoldCount < 500) {
		m_vButtons[btn::iceArrowBuy]->SetButtonState(true);
	}
	else {
		m_vButtons[btn::iceArrowBuy]->SetButtonState(false);
	}

	if (State::m_iGoldCount < 800) {
		m_vButtons[btn::treeRootArrowBuy]->SetButtonState(true);
	}
	else {
		m_vButtons[btn::treeRootArrowBuy]->SetButtonState(false);
	}

	if (State::m_iGoldCount < 1200) {
		m_vButtons[btn::fireCannonBuy]->SetButtonState(true);
	}
	else {
		m_vButtons[btn::fireCannonBuy]->SetButtonState(false);
	}

	if (State::m_iGoldCount < 1400) {
		m_vButtons[btn::iceCarnnonBuy]->SetButtonState(true);
	}
	else {
		m_vButtons[btn::iceCarnnonBuy]->SetButtonState(false);
	}

	if (State::m_iGoldCount < 2000) {
		m_vButtons[btn::misslesBuy]->SetButtonState(true);
	}
	else {
		m_vButtons[btn::misslesBuy]->SetButtonState(false);
	}
	
	if (m_vButtons[btn::fireArrowBuy]->Clicked() && (State::m_vWeapons.size() < 4) && !m_bButtonClick) {
		m_iGoldCount -= 300;
		State::m_vWeapons.push_back(1);
		m_bButtonClick = true;
	}
	else if (m_vButtons[btn::iceArrowBuy]->Clicked() && (State::m_vWeapons.size() < 4) && !m_bButtonClick) {
		m_iGoldCount -= 500;
		State::m_vWeapons.push_back(2);
		m_bButtonClick = true;
	}
	else if (m_vButtons[btn::treeRootArrowBuy]->Clicked() && (State::m_vWeapons.size() < 4) && !m_bButtonClick) {
		m_iGoldCount -= 800;
		State::m_vWeapons.push_back(3);
		m_bButtonClick = true;
	}
	else if (m_vButtons[btn::fireCannonBuy]->Clicked() && (State::m_vWeapons.size() < 4) && !m_bButtonClick) {
		m_iGoldCount -= 1200;
		State::m_vWeapons.push_back(4);
		m_bButtonClick = true;
	}
	else if (m_vButtons[btn::iceCarnnonBuy]->Clicked() && (State::m_vWeapons.size() < 4) && !m_bButtonClick) {
		m_iGoldCount -= 1400;
		State::m_vWeapons.push_back(5);
		m_bButtonClick = true;
	}
	else if (m_vButtons[btn::misslesBuy]->Clicked() && (State::m_vWeapons.size() < 4) && !m_bButtonClick) {
		m_iGoldCount -= 2000;
		State::m_vWeapons.push_back(6);
		m_bButtonClick = true;
	}
}

void ShopState::Render()
{
	if (State::m_iDebugSet)
		cout << "[Shop] Rendering Shop Screen..." << endl;

	SDL_Rect m_rSrc = { 0,0,1024,768 };
	SDL_Rect m_rDst = { 0,0,1024,768 };
	SDL_RenderCopy(Game::Instance()->GetRenderer(), m_pText, &m_rSrc, &m_rDst);

	SDL_SetRenderDrawColor(Game::Instance()->GetRenderer(), 255, 255, 255, 250);
	SDL_Rect m_rOverlay = { 100, 100, 824, 588 };
	SDL_Color m_cCol = { 218, 165, 32, 0 };
	string tString = "Available Gold: " + to_string(State::m_iGoldCount);
	RenderColorFont(1, tString.c_str(), 430, 65, m_cCol);
	SDL_RenderDrawRect(Game::Instance()->GetRenderer(), &m_rOverlay);

	for (int i = 0; i < m_vButtons.size(); i++)
	{
		m_vButtons[i]->Render();
	}

	SDL_Rect m_rPanelSrc = { 0, 0, 100, 100 };
	SDL_Rect m_rWeaponSrc = { 0, 0, 32, 90 };
	SDL_Rect m_rFirstPanel = { 265, 120, 100, 100 };
	SDL_RenderCopy(Game::Instance()->GetRenderer(), m_pPanel, &m_rPanelSrc, &m_rFirstPanel);
	SDL_Rect m_rFirstWeapon = { 298, 124, 32, 90 };
	SDL_RenderCopy(Game::Instance()->GetRenderer(), m_pFireArrow, &m_rWeaponSrc, &m_rFirstWeapon);
	RenderColorFont(1, "Fire Arrow (300 Gold)", 190, 225, { 255, 0, 0, 0 });

	SDL_Rect m_rSecondPanel = { 645, 120, 100, 100 };
	SDL_RenderCopy(Game::Instance()->GetRenderer(), m_pPanel, &m_rPanelSrc, &m_rSecondPanel);
	SDL_Rect m_rSecondWeapon = { 678, 124, 32, 90 };
	SDL_RenderCopy(Game::Instance()->GetRenderer(), m_pIceArrow, &m_rWeaponSrc, &m_rSecondWeapon);
	RenderColorFont(1, "Ice Arrow (500 Gold)", 570, 225, { 255, 0, 0, 0 });

	SDL_Rect m_rThirdPanel = { 265, 305, 100, 100 };
	SDL_RenderCopy(Game::Instance()->GetRenderer(), m_pPanel, &m_rPanelSrc, &m_rThirdPanel);
	SDL_Rect m_rThirdWeapon = { 298, 309, 32, 90 };
	SDL_RenderCopy(Game::Instance()->GetRenderer(), m_pRootArrow, &m_rWeaponSrc, &m_rThirdWeapon);
	RenderColorFont(1, "Tree Root Arrow (800 Gold)", 160, 410, { 255, 0, 0, 0 });
	//RenderFont(1, "Tree Root Arrow", 230, 410);

	SDL_Rect m_rWeaponSrc2 = { 0, 0, 89, 90 };
	SDL_Rect m_rFourthPanel = { 645, 305, 100, 100 };
	SDL_RenderCopy(Game::Instance()->GetRenderer(), m_pPanel, &m_rPanelSrc, &m_rFourthPanel);
	SDL_Rect m_rFourthWeapon = { 652, 309, 89, 90 };
	SDL_RenderCopy(Game::Instance()->GetRenderer(), m_pFireCannon, &m_rWeaponSrc2, &m_rFourthWeapon);
	RenderColorFont(1, "Fire Cannon (1200 Gold)", 570, 410, { 255, 0, 0, 0 });

	SDL_Rect m_rFifthPanel = { 265, 490, 100, 100 };
	SDL_RenderCopy(Game::Instance()->GetRenderer(), m_pPanel, &m_rPanelSrc, &m_rFifthPanel);
	SDL_Rect m_rFifthWeapon = { 272, 494, 89, 90 };
	SDL_RenderCopy(Game::Instance()->GetRenderer(), m_pIceCannon, &m_rWeaponSrc2, &m_rFifthWeapon);
	RenderColorFont(1, "Ice Cannon (1400 Gold)", 190, 595, { 255, 0, 0, 0 });

	SDL_Rect m_rWeaponSrc3 = { 0, 0, 80, 75 };
	SDL_Rect m_rSixthPanel = { 645, 490, 100, 100 };
	SDL_RenderCopy(Game::Instance()->GetRenderer(), m_pPanel, &m_rPanelSrc, &m_rSixthPanel);
	SDL_Rect m_rSixthWeapon = { 652, 498, 80, 75 };
	SDL_RenderCopy(Game::Instance()->GetRenderer(), m_pMissile, &m_rWeaponSrc3, &m_rSixthWeapon);
	RenderColorFont(1, "Missile (2000 Gold)", 590, 595, { 255, 0, 0, 0 });

	State::Render();
}

void ShopState::Enter()
{
	if (State::m_iDebugSet)
		cout << "[ShopState] Entering Shop..." << endl;
	
	m_Font = TTF_OpenFont("Assets/imgs/LTYPE.TTF", 20);

	m_vButtons.push_back(new Button("Assets/imgs/buttons/closeshop.png", { (1024 / 2) + 100, 700, 350, 46 }, "Assets/sounds/btnHover.wav", "Assets/sounds/btnClick.wav"));
	m_vButtons.push_back(new Button("Assets/imgs/buttons/buyitem.png", { 140, 250, 350, 46 }, "Assets/sounds/btnHover.wav", "Assets/sounds/btnClick.wav"));
	m_vButtons.push_back(new Button("Assets/imgs/buttons/buyitem.png", { 520, 250, 350, 46 }, "Assets/sounds/btnHover.wav", "Assets/sounds/btnClick.wav"));
	m_vButtons.push_back(new Button("Assets/imgs/buttons/buyitem.png", { 140, 435, 350, 46 }, "Assets/sounds/btnHover.wav", "Assets/sounds/btnClick.wav"));
	m_vButtons.push_back(new Button("Assets/imgs/buttons/buyitem.png", { 520, 435, 350, 46 }, "Assets/sounds/btnHover.wav", "Assets/sounds/btnClick.wav"));
	m_vButtons.push_back(new Button("Assets/imgs/buttons/buyitem.png", { 140, 620, 350, 46 }, "Assets/sounds/btnHover.wav", "Assets/sounds/btnClick.wav"));
	m_vButtons.push_back(new Button("Assets/imgs/buttons/buyitem.png", { 520, 620, 350, 46 }, "Assets/sounds/btnHover.wav", "Assets/sounds/btnClick.wav"));

	Game::Instance()->GetAM()->ClearMusic();
	Game::Instance()->GetAM()->LoadMusic("Assets/sounds/shopmusic.mp3");
	Game::Instance()->GetAM()->PlayMusic();

	m_pSurf = IMG_Load("Assets/imgs/title.jpg");
	m_pText = SDL_CreateTextureFromSurface(Game::Instance()->GetRenderer(), m_pSurf);
	SDL_FreeSurface(m_pSurf);

	m_pSurf = IMG_Load("Assets/imgs/panel.png");
	m_pPanel = SDL_CreateTextureFromSurface(Game::Instance()->GetRenderer(), m_pSurf);
	SDL_FreeSurface(m_pSurf);

	m_pSurf = IMG_Load("Assets/imgs/firearrow.png");
	m_pFireArrow = SDL_CreateTextureFromSurface(Game::Instance()->GetRenderer(), m_pSurf);
	SDL_FreeSurface(m_pSurf);

	m_pSurf = IMG_Load("Assets/imgs/icearrow.png");
	m_pIceArrow = SDL_CreateTextureFromSurface(Game::Instance()->GetRenderer(), m_pSurf);
	SDL_FreeSurface(m_pSurf);

	m_pSurf = IMG_Load("Assets/imgs/treerootarrow.png");
	m_pRootArrow = SDL_CreateTextureFromSurface(Game::Instance()->GetRenderer(), m_pSurf);
	SDL_FreeSurface(m_pSurf);

	m_pSurf = IMG_Load("Assets/imgs/firecannon.png");
	m_pFireCannon = SDL_CreateTextureFromSurface(Game::Instance()->GetRenderer(), m_pSurf);
	SDL_FreeSurface(m_pSurf);

	m_pSurf = IMG_Load("Assets/imgs/icecannon.png");
	m_pIceCannon = SDL_CreateTextureFromSurface(Game::Instance()->GetRenderer(), m_pSurf);
	SDL_FreeSurface(m_pSurf);

	m_pSurf = IMG_Load("Assets/imgs/missile.png");
	m_pMissile = SDL_CreateTextureFromSurface(Game::Instance()->GetRenderer(), m_pSurf);
	SDL_FreeSurface(m_pSurf);

}

void ShopState::Exit()
{
	Game::Instance()->GetAM()->ToggleMusic();
	Game::Instance()->GetAM()->ClearMusic();
	for (int i = 0; i < m_vButtons.size(); i++)
	{
		delete m_vButtons[i];
		m_vButtons.erase(m_vButtons.begin() + i);
	}
	SDL_DestroyTexture(m_pText);
	SDL_DestroyTexture(m_pFireArrow);
	SDL_DestroyTexture(m_pIceArrow);
	SDL_DestroyTexture(m_pRootArrow);
	SDL_DestroyTexture(m_pFireCannon);
	SDL_DestroyTexture(m_pIceCannon);
	SDL_DestroyTexture(m_pMissile);
	TTF_CloseFont(m_Font);
}
