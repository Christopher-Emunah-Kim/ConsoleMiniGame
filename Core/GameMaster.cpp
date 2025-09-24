#include "GameMaster.h"

void GameMaster::Initialize()
{
	if(!m_inputService)
		m_inputService = make_unique<InputService>();

	if (!m_timeService)
		m_timeService = make_unique<TimeService>();

	if(!m_contentManager)
		m_contentManager = make_unique<ContentManager>();

	if ( !m_hud )
		m_hud = make_unique<HUD>();
}

void GameMaster::Shutdown()
{
	if(m_hud)
	{
		m_hud.reset();
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
