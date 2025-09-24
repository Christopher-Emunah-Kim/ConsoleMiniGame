#pragma once
#include "../Core/Common.h"
#include "../Core/Singleton.h"
#include "../Service/InputService.h"
#include "../Service/TimeService.h"
#include "../Main/ContentManager.h"
#include "../Main/HUD.h"
#include "GameWorld.h"

class ScreenService;

class GameMaster final : public Singleton<GameMaster>
{
public:
	GameMaster() = default;
	virtual ~GameMaster() = default;

	void Initialize();
	void Shutdown();

	void SetScreen( ScreenService* screen );
	ScreenService& GetScreen() const;

	ContentManager& GetContentManager() const { return *m_contentManager; }
	InputService& GetInputService() const { return *m_inputService; }
	TimeService& GeteTimeService() const { return *m_timeService; }
	HUD& GetHUD() const { return *m_hud; }

private:
	unique_ptr<ContentManager> m_contentManager;
	unique_ptr<InputService> m_inputService;
	unique_ptr<TimeService> m_timeService;
	unique_ptr<HUD> m_hud;

	ScreenService* m_screen = nullptr;
};

