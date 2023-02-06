#pragma once
#ifndef __PLAY_SCENE__
#define __PLAY_SCENE__

#include "Scene.h"
#include "Plane.h"
#include "Player.h"
#include "Button.h"
#include "Label.h"

#include "Target.h"
#include "ship.h"
#include "StarShip.h"


class PlayScene : public Scene
{
public:
	PlayScene();
	~PlayScene() override;

	// Scene LifeCycle Functions
	virtual void Draw() override;
	virtual void Update() override;
	virtual void Clean() override;
	virtual void HandleEvents() override;
	virtual void Start() override;
private:

	StarShip* m_pStarShip;
	Ship* m_pShip;

	// IMGUI Function
	void GUI_Function();
	std::string m_guiTitle;

	glm::vec2 m_mousePosition;
	Target* m_pTarget;

	Plane* m_pPlaneSprite{};
	Player* m_pPlayer{};
	bool m_playerFacingRight{};

	// UI Items
	Button* m_pBackButton{};
	Button* m_pNextButton{};
	Label* m_pInstructionsLabel{};

	// Input Control
	int m_pCurrentInputType{};
	void GetPlayerInput();
	void GetKeyboardInput();


	//debug variable

	bool m_bDebugView;
};

#endif /* defined (__PLAY_SCENE__) */