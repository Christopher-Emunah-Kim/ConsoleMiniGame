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
	GAME_MASTER->GetWorld().Update();
}

void GameApplication::Render()
{
	m_screen.Clear();
	//TODO : GAMEINSTANCE 쓸건지 말건지 결정 필요
	//GameInstance::GetInstance()->RenderHud(m_screen);
	m_screen.SwapBuffer();
}


void GameApplication::Release()
{
	GAME_MASTER->GetWorld().Release();
}

bool GameApplication::Initialize()
{
	m_screen.Init();
	GAME_MASTER->GetWorld().Init();

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
