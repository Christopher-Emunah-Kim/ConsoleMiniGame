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

struct FHudLayout final
{
	int32 infoPanelWidth = PLAYERINFO_PANEL_WIDTH;
	int32 mainPanelStartX = GAME_PANEL_START_X;
	int32 mainPanelTopY = SYSTEM_TEXT_BASE_Y + 1;
	int32 mainPanelHeight = MAX_LINES;
	int32 rightMargin = RIGHT_MARGIN;
	bool useMainTextPanel = true;

};

struct FHudViewport final
{
	int32 x = GAME_PANEL_START_X + 1;
	int32 y = SYSTEM_TEXT_BASE_Y + 1;
	int32 width = SCREEN_WIDTH - RIGHT_MARGIN - ( GAME_PANEL_START_X + 1 );
	int32 height = MAX_LINES;
};

class HUD final
{
public:
	HUD();
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
	void SetCommandLineText( const wstring& text );

	void SetLayout( const FHudLayout& layout );
	void ResetLayout();

	FHudLayout& GetLayout() { return m_layout; }
	FHudLayout& GetDefaultLayout() { return m_defaultLayout; }
	FHudViewport GetGameViewportRect() const;

private:
	FHudData m_hudData;
	FHudLayout m_layout;
	FHudLayout m_defaultLayout;
	deque<wstring> m_mainTextQueue;
	wstring m_commandLineText;
};

