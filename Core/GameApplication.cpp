#include "GameApplication.h"
#include "../Service/TimeService.h"
#include "GameMaster.h"

GameApplication::~GameApplication() noexcept
{
	Release();
}

void GameApplication::ProcessInput()
{
	GAME_MASTER->GetInputService().ProcessTextInput();
}


void GameApplication::Update()
{
	GAME_MASTER->GetContentManager().OnUpdate();
}

void GameApplication::Render()
{
	m_screen.Clear();
	GAME_MASTER->GetContentManager().OnRender();
	GAME_MASTER->GetHUD().Render( m_screen );
	m_screen.SwapBuffer();
}


void GameApplication::Release()
{
	GAME_MASTER->Shutdown();
	m_screen.Release();
}

bool GameApplication::Initialize()
{
	m_screen.Init();
	GAME_MASTER->Initialize();
	GAME_MASTER->GetContentManager().LoadContent();

	return true;
}

void GameApplication::Run()
{
	TimeService& timeService = GAME_MASTER->GeteTimeService();
	timeService.Init();

	//while (!GameInstance()->ShouldQuit())
	while (true)
	{
		if (timeService.CanUpdate())
		{
			ProcessInput();
			Update();
			Render();
		}
	}
}
