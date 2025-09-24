#pragma once
#include "../Service/ScreenService.h"

constexpr __int32 MAX_LINES = 35;
constexpr __int32 DEFAULT_BASE_Y = SCREEN_HEIGHT - MAX_LINES - 4;
constexpr __int8 LEFT_MARGIN = 3;
constexpr __int8 RIGHT_MARGIN = 4;
constexpr __int8 COMMAND_BLOCK_HEIGHT = 3;
constexpr __int16 PLAYER_UI_BASE_Y = 1;
constexpr __int16 SYSTEM_TEXT_BASE_Y = 3;

class ScreenService;

struct FHudData final
{
	wstring contentName;
	wstring playerName;
};

class HUD final
{
public:
	HUD() = default;
	virtual ~HUD() = default;

private:
	void RenderOutFrame( ScreenService& screen ) const;
	void RenderInfoPanel(ScreenService& screen) const;
	void RenderMainPanel(ScreenService& screen) const;
	void RenderBottomPanel(ScreenService& screen) const;

public:
	void Render(ScreenService& screen) const;
	void WriteLine(const wstring& line);
	void ClearText();

	//void UpdateHudData(const wstring& contentName, const wstring& playerName);

private:
	FHudData m_hudData;
	deque<wstring> m_mainTextQueue;
};

