#include "GameApplication.h"
#include "../Service/TimeService.h"

GameApplication::~GameApplication() noexcept
{
	Release();
}

void GameApplication::ProcessInput()
{
	GameMaster::GetIntance()->GetInputService().ProcessTextInput();
}

void GameApplication::Update()
{
	GameMaster::GetIntance()->GetWorld().Update();
}

void GameApplication::Render()
{
	m_screen.Clear();
	GameInstance::GetInstance()->RenderHud(m_screen);
	m_screen.SwapBuffer();
}

void GameApplication::Release()
{
	GameMaster::GetInstance()->GetWorld().Shutdown();
}

bool GameApplication::Initialize()
{
	m_screen.Init();
	GameMaster::GetInstance()->GetWorld().Initialize();

	return true;
}

void GameApplication::Run()
{
	TimeService& timeService = GameMaster::GetIntance()->GeteTimeService();
	timeService.Init();

	while (!GameInstance()->ShouldQuit())
	{
		if (timeService.CanUpdate())
		{
			ProcessInput();
			Update();
			Render();
		}
	}
}
