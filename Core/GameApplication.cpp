#include "GameApplication.h"
#include "../Service/TimeService.h"
#include "GameMaster.h"

GameApplication::~GameApplication() noexcept
{
	Release();
}

void GameApplication::ProcessInput()
{
	GameMaster::GetInstance()->GetInputService().ProcessTextInput();
}


void GameApplication::Update()
{
	GameMaster::GetInstance()->GetWorld().Update();
}

void GameApplication::Render()
{
	m_screen.Clear();
	//GameInstance::GetInstance()->RenderHud(m_screen);
	m_screen.SwapBuffer();
}


void GameApplication::Release()
{
	GameMaster::GetInstance()->GetWorld().Release();
}

bool GameApplication::Initialize()
{
	m_screen.Init();
	GameMaster::GetInstance()->GetWorld().Init();

	return true;
}

void GameApplication::Run()
{
	TimeService& timeService = GameMaster::GetInstance()->GeteTimeService();
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
