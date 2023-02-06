#include "PlayScene.h"
#include "Game.h"
#include "EventManager.h"
#include "InputType.h"
#include "StarShip.h"

// required for IMGUI
#include "imgui.h"
#include "imgui_sdl.h"
#include "Renderer.h"
#include "Util.h"
//#include "../../../GAME3001_W01_Project/GAME3001_W01_Project/src/StarShip.h"

PlayScene::PlayScene()
{
	PlayScene::Start();
}

PlayScene::~PlayScene()
= default;

void PlayScene::Draw()
{
	DrawDisplayList();
	SDL_SetRenderDrawColor(Renderer::Instance().GetRenderer(), 255, 255, 255, 255);
}

void PlayScene::Update()
{
	UpdateDisplayList();
}

void PlayScene::Clean()
{
	RemoveAllChildren();
}


void PlayScene::HandleEvents()
{
	EventManager::Instance().Update();

	GetPlayerInput();

	GetKeyboardInput();
}

void PlayScene::GetPlayerInput()
{
	switch (m_pCurrentInputType)
	{
	case static_cast<int>(InputType::GAME_CONTROLLER):
	{
		// handle player movement with GameController
		if (SDL_NumJoysticks() > 0)
		{
			if (EventManager::Instance().GetGameController(0) != nullptr)
			{
				constexpr auto dead_zone = 10000;
				if (EventManager::Instance().GetGameController(0)->STICK_LEFT_HORIZONTAL > dead_zone)
				{
					m_pPlayer->SetAnimationState(PlayerAnimationState::PLAYER_RUN_RIGHT);
					m_playerFacingRight = true;
				}
				else if (EventManager::Instance().GetGameController(0)->STICK_LEFT_HORIZONTAL < -dead_zone)
				{
					m_pPlayer->SetAnimationState(PlayerAnimationState::PLAYER_RUN_LEFT);
					m_playerFacingRight = false;
				}
				else
				{
					if (m_playerFacingRight)
					{
						m_pPlayer->SetAnimationState(PlayerAnimationState::PLAYER_IDLE_RIGHT);
					}
					else
					{
						m_pPlayer->SetAnimationState(PlayerAnimationState::PLAYER_IDLE_LEFT);
					}
				}
			}
		}
	}
	break;
	case static_cast<int>(InputType::KEYBOARD_MOUSE):
	{
		// handle player movement with mouse and keyboard
		if (EventManager::Instance().IsKeyDown(SDL_SCANCODE_A))
		{
			m_pPlayer->SetAnimationState(PlayerAnimationState::PLAYER_RUN_LEFT);
			m_playerFacingRight = false;
		}
		else if (EventManager::Instance().IsKeyDown(SDL_SCANCODE_D))
		{
			m_pPlayer->SetAnimationState(PlayerAnimationState::PLAYER_RUN_RIGHT);
			m_playerFacingRight = true;
		}
		else
		{
			if (m_playerFacingRight)
			{
				m_pPlayer->SetAnimationState(PlayerAnimationState::PLAYER_IDLE_RIGHT);
			}
			else
			{
				m_pPlayer->SetAnimationState(PlayerAnimationState::PLAYER_IDLE_LEFT);
			}
		}
	}
	break;
	case static_cast<int>(InputType::ALL):
	{
		if (SDL_NumJoysticks() > 0)
		{
			if (EventManager::Instance().GetGameController(0) != nullptr)
			{
				constexpr auto dead_zone = 10000;
				if (EventManager::Instance().GetGameController(0)->STICK_LEFT_HORIZONTAL > dead_zone
					|| EventManager::Instance().IsKeyDown(SDL_SCANCODE_D))
				{
					m_pPlayer->SetAnimationState(PlayerAnimationState::PLAYER_RUN_RIGHT);
					m_playerFacingRight = true;
				}
				else if (EventManager::Instance().GetGameController(0)->STICK_LEFT_HORIZONTAL < -dead_zone
					|| EventManager::Instance().IsKeyDown(SDL_SCANCODE_A))
				{
					m_pPlayer->SetAnimationState(PlayerAnimationState::PLAYER_RUN_LEFT);
					m_playerFacingRight = false;
				}
				else
				{
					if (m_playerFacingRight)
					{
						m_pPlayer->SetAnimationState(PlayerAnimationState::PLAYER_IDLE_RIGHT);
					}
					else
					{
						m_pPlayer->SetAnimationState(PlayerAnimationState::PLAYER_IDLE_LEFT);
					}
				}
			}
		}
		else if (EventManager::Instance().IsKeyDown(SDL_SCANCODE_A))
		{
			m_pPlayer->SetAnimationState(PlayerAnimationState::PLAYER_RUN_LEFT);
			m_playerFacingRight = false;
		}
		else if (EventManager::Instance().IsKeyDown(SDL_SCANCODE_D))
		{
			m_pPlayer->SetAnimationState(PlayerAnimationState::PLAYER_RUN_RIGHT);
			m_playerFacingRight = true;
		}
		else
		{
			if (m_playerFacingRight)
			{
				m_pPlayer->SetAnimationState(PlayerAnimationState::PLAYER_IDLE_RIGHT);
			}
			else
			{
				m_pPlayer->SetAnimationState(PlayerAnimationState::PLAYER_IDLE_LEFT);
			}
		}
	}
	break;
	}
}

void PlayScene::GetKeyboardInput()
{
	if (EventManager::Instance().IsKeyDown(SDL_SCANCODE_ESCAPE))
	{
		Game::Instance().Quit();
	}

	if (EventManager::Instance().IsKeyDown(SDL_SCANCODE_1))
	{
		//Game::Instance().ChangeSceneState(SceneState::START);
		//seekmode = true;
	}

	if (EventManager::Instance().IsKeyDown(SDL_SCANCODE_2))
	{
		Game::Instance().ChangeSceneState(SceneState::END);
	}
}

void PlayScene::Start()
{
	// add sound mnaager function






	// Set GUI Title
	m_guiTitle = "Play Scene";
	m_bDebugView = false; // turn off all colliders

	// Player Sprite
	m_pPlayer = new Player();
	m_pPlayer->GetTransform()->position = glm::vec2(50.0f, 50.0f);
	AddChild(m_pPlayer);

	m_playerFacingRight = true;

	m_pTarget = new Target();

	m_pTarget->GetTransform()->position = glm::vec2(500.0f, 100.0f);

	AddChild(m_pTarget);

	m_pShip = new Ship();
	AddChild(m_pShip);

	m_pStarShip = new StarShip();
	m_pStarShip->GetTransform()->position = glm::vec2(100.0f, 400.0f);
	m_pStarShip->SetTargetPosition(m_pTarget->GetTransform()->position);
	//m_pStarShip->GetCurrentDirection(glm::vec2(1.0f, 0.0f));
	//m_pStarShip->SetEnabled(false);
	AddChild(m_pStarShip);

	SoundManager::Instance().Load("../Assets/Audio/yay.ogg", "yay", SoundType::SOUND_SFX);




	/* DO NOT REMOVE */
	ImGuiWindowFrame::Instance().SetGuiFunction([this] { GUI_Function(); });
}

void PlayScene::GUI_Function() 
{
	// Always open with a NewFrame
	ImGui::NewFrame();

	// See examples by uncommenting the following - also look at imgui_demo.cpp in the IMGUI filter
	//ImGui::ShowDemoWindow();
	
	ImGui::Begin("Your Window Title Goes Here", nullptr, ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoMove);

	ImGui::Text("Player Input");
	ImGui::RadioButton("Keyboard / Mouse", &m_pCurrentInputType, static_cast<int>(InputType::KEYBOARD_MOUSE)); ImGui::SameLine();
	ImGui::RadioButton("Game Controller", &m_pCurrentInputType, static_cast<int>(InputType::GAME_CONTROLLER)); ImGui::SameLine();
	ImGui::RadioButton("Both", &m_pCurrentInputType, static_cast<int>(InputType::ALL));

	ImGui::Separator();

	if(ImGui::Button("My Button"))
	{
		std::cout << "My Button Pressed" << std::endl;
	}

	ImGui::Separator();

	static float float3[3] = { 0.0f, 1.0f, 1.5f };
	if(ImGui::SliderFloat3("My Slider", float3, 0.0f, 2.0f))
	{
		std::cout << float3[0] << std::endl;
		std::cout << float3[1] << std::endl;
		std::cout << float3[2] << std::endl;
		std::cout << "---------------------------\n";
	}
	
	ImGui::End();
}
