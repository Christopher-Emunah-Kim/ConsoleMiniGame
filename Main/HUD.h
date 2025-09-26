#pragma once
#include "../Service/ScreenService.h"

class ScreenService;

//HUD 텍스트/명령패널 배치 레이아웃
struct FHudLayout final
{
	int32 mainPanelStartX = SCREEN_WIDTH / 4;
	int32 mainPanelTopY = 2;
	int32 mainPanelHeight = SCREEN_HEIGHT - 5;
	int32 rightMargin = 4;
	bool bEnableMainPanel = false;
	bool bEnableBottomPanel = false;
};

//메인 게임화면 레이아웃
struct FHudViewport final
{
	int32 x;
	int32 y;
	int32 width;
	int32 height;
};

class HUD final
{
public:
	HUD();
	virtual ~HUD() = default;

private:
	void RenderOutFrame( ScreenService& screen ) const;
	void RenderMainPanel(ScreenService& screen) const;
	void RenderBottomPanel(ScreenService& screen) const;

public:
	void Render(ScreenService& screen) const;
	void WriteLine(const wstring& line);
	void ClearText();
	void SetCommandLineText( const wstring& text );

	void SetLayout( const FHudLayout& layout );
	void ResetLayout();
	
	void EnableMainPanel( bool bEnable );
	void EnableCommandLine( bool bEnable );

	FHudLayout& GetLayout() { return m_layout; }
	FHudLayout& GetDefaultLayout() { return m_defaultLayout; }
	FHudViewport GetGameViewportRect() const;

private:
	static constexpr int32 COMMAND_PANEL_HEIGHT = 3;
	FHudLayout m_layout;
	FHudLayout m_defaultLayout;

	deque<wstring> m_mainTextQueue;
	wstring m_commandLineText;
};

