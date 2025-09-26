#include "HUD.h"
#include "../Core/GameMaster.h"
#include <algorithm>


HUD::HUD()
{
	m_defaultLayout = FHudLayout();
	m_layout = m_defaultLayout;
}

void HUD::Render(ScreenService& screen) const
{
	RenderOutFrame( screen );

	if ( m_layout.bEnableMainPanel )
	{
		RenderMainPanel(screen);
	}

	if(m_layout.bEnableBottomPanel)
	{
		RenderBottomPanel(screen);
	}
}


void HUD::RenderOutFrame( ScreenService& screen ) const
{
	const int left = 0;                        
	const int right = SCREEN_WIDTH - 2;        
	const int top = 0;                        
	const int bottom = SCREEN_HEIGHT - 1;        

	const int innerWidth = max(0, right - left - 1); // 가로 길이(모서리 제외)

	//Top Line: ┌ + ─ * innerW + ┐
	{
		std::wstring line;
		line.reserve( static_cast<size_t>( innerWidth + 2 ) );

		line.push_back( L'┌' );
		if ( innerWidth > 0 )
		{
			line.append( static_cast<size_t>( innerWidth ) , L'─' );
		}
		line.push_back( L'┐' );

		screen.Draw( left , top , line );    
	}

	//Side Line: y = top+1 .. bottom-1
	for ( int y = top + 1; y <= bottom - 1; ++y )
	{
		screen.Draw( left , y , L"│" );     
		screen.Draw( right , y , L"│" );           
	}

	//Bottom Line: └ + ─ * innerW + ┘
	{
		std::wstring line;
		line.reserve( static_cast<size_t>( innerWidth + 2 ) );

		line.push_back( L'└' );
		if ( innerWidth > 0 )
		{
			line.append( static_cast<size_t>( innerWidth ) , L'─' );
		}
		line.push_back( L'┘' );

		screen.Draw( left , bottom , line ); 
	}
}

// Render Wirted Line Text
void HUD::RenderMainPanel(ScreenService& screen) const
{
	const int32 startX = m_layout.mainPanelStartX + 1;
	const int32 startY = m_layout.mainPanelTopY;
	const int32 height = m_layout.mainPanelHeight;
	
	if(height <= 0)
	{
		return;
	}
	
	const int32 messageAreaWidth = max( 0 , SCREEN_WIDTH - m_layout.mainPanelStartX - m_layout.rightMargin );
	const wstring clearLine( static_cast<size_t>( messageAreaWidth ) , L' ' );

	// Clear Area
	for ( int32 offsetY = 0; offsetY < height; ++offsetY )
	{
		screen.Draw( startX , startY + offsetY , clearLine );
	}

	// Draw Text
	int32 outputY = startY;
	for (size_t i = 0; i < m_mainTextQueue.size(); ++i)
	{
		screen.Draw(startX, outputY, m_mainTextQueue[i]);
		++outputY;
	}
}

void HUD::RenderBottomPanel(ScreenService& screen) const
{
	const int32 commandLineY = SCREEN_HEIGHT - COMMAND_PANEL_HEIGHT + 1;
	const int32 clearWidth = max( 0 , SCREEN_WIDTH - m_layout.rightMargin - 1 );

	const wstring clearLine( static_cast<size_t>( clearWidth ) , L' ' );

	screen.Draw( 1 , commandLineY , clearLine );

	const wstring prompt = L"명령 > ";
	const int32 maxLength = max( 0 , SCREEN_WIDTH - m_layout.rightMargin - 10 );

	wstring displayText = m_commandLineText.substr( 0 , static_cast<size_t>( maxLength ) );

	screen.Draw( 2 , commandLineY , prompt + displayText );
}

void HUD::WriteLine(const wstring& line)
{
	const int32 MAX_LINES = static_cast<int32>( m_layout.mainPanelHeight );

	if (static_cast<int32>(m_mainTextQueue.size()) >= MAX_LINES)
	{
		m_mainTextQueue.pop_front();
	}

	m_mainTextQueue.push_back(line);
}

void HUD::ClearText()
{
	m_mainTextQueue.clear();
	m_commandLineText.clear();
}

void HUD::SetCommandLineText( const wstring& text )
{
	m_commandLineText = text;
}

void HUD::SetLayout( const FHudLayout& layout )
{
	m_layout = layout;

	//레이아웃 변경시 텍스트 큐 크기 조정
	const int32 maxLInes = max( 1 , m_layout.mainPanelHeight );

	while(static_cast<int32>(m_mainTextQueue.size()) > maxLInes)
	{
		m_mainTextQueue.pop_front();
	}
}

void HUD::ResetLayout()
{
	m_layout = m_defaultLayout;
}

void HUD::EnableMainPanel( bool bEnable )
{
	m_layout.bEnableMainPanel = bEnable;
}

void HUD::EnableCommandLine( bool bEnable )
{
	m_layout.bEnableBottomPanel = bEnable;
}

FHudViewport HUD::GetGameViewportRect() const
{
	FHudViewport viewport{};

	viewport.x = m_layout.mainPanelStartX + 1;
	viewport.y = m_layout.mainPanelTopY;
	viewport.width = max( 0 , SCREEN_WIDTH - m_layout.rightMargin - viewport.x );
	viewport.height = m_layout.mainPanelHeight;

	return viewport;
}
