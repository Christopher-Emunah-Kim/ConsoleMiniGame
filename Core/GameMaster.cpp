#include "GameMaster.h"

void GameMaster::Initialize()
{
	if(!m_inputService)
		m_inputService = make_unique<InputService>();

	if (!m_timeService)
		m_timeService = make_unique<TimeService>();

	if(!m_contentManager)
		m_contentManager = make_unique<ContentManager>();

	if (!m_gameWorld)
		m_gameWorld = make_unique<GameWorld>();
}

void GameMaster::Shutdown()
{
	if(m_gameWorld)
	{
		m_gameWorld->Release();
		m_gameWorld.reset();
	}

	if(m_contentManager)
	{
		if(m_contentManager->GetCurrentContent())
		{
			m_contentManager->GetCurrentContent()->OnRelease();
		}
		m_contentManager.reset();
	}

	if(m_timeService)
	{
		m_timeService.reset();
	}

	if(m_inputService)
	{
		m_inputService->Clear();
		m_inputService.reset();
	}
}
