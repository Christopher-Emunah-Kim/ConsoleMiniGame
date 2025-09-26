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
	//RenderInfoPanel(screen);
	RenderMainPanel(screen);
	RenderBottomPanel(screen);
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
		if ( innerWidth > 0 ) line.append( static_cast<size_t>( innerWidth ) , L'─' );
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
		if ( innerWidth > 0 ) line.append( static_cast<size_t>( innerWidth ) , L'─' );
		line.push_back( L'┘' );
		screen.Draw( left , bottom , line ); 
	}
}

//TODO : 도규꺼 보고 Render 일괄 처리할수있는 프레임 구조 없나 확인. 이건 너무 하드코딩 느낌.
void HUD::RenderInfoPanel(ScreenService& screen) const
{

	screen.Draw(LEFT_MARGIN + 5, 1, L"[ WELCOM TO TRPG ]");
	screen.Draw((int32)(SCREEN_WIDTH * 0.55f), 1, L" ");
	screen.Draw(LEFT_MARGIN - 2, PLAYER_UI_BASE_Y + 1, L"───────────────────────────────");
	screen.Draw(m_layout.mainPanelStartX, PLAYER_UI_BASE_Y + 1, L"────────────────────────────────────────────────────────────────────────────────────────────");
	screen.Draw(LEFT_MARGIN, PLAYER_UI_BASE_Y + 3, L"     [ 플레이어  정보 ]");
	screen.Draw(LEFT_MARGIN - 2, PLAYER_UI_BASE_Y + 4, L"───────────────────────────────");
	screen.Draw(LEFT_MARGIN - 2, PLAYER_UI_BASE_Y + 10, L"───────────────────────────────");
	screen.Draw(LEFT_MARGIN - 2, PLAYER_UI_BASE_Y + 18, L"───────────────────────────────");
	screen.Draw(LEFT_MARGIN, PLAYER_UI_BASE_Y + 19, L"  [ 플레이어  스테이터스 ]");
	screen.Draw(LEFT_MARGIN - 2, PLAYER_UI_BASE_Y + 20, L"───────────────────────────────");
	screen.Draw(LEFT_MARGIN - 2, PLAYER_UI_BASE_Y + 28, L"───────────────────────────────");
	screen.Draw(0, SCREEN_HEIGHT - COMMAND_BLOCK_HEIGHT, L"│─────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────│");
	screen.Draw(0, SCREEN_HEIGHT - COMMAND_BLOCK_HEIGHT + 1, L"│");
	screen.Draw(SCREEN_WIDTH - 2, SCREEN_HEIGHT - COMMAND_BLOCK_HEIGHT + 1, L"│");
	
	screen.Draw( 2 , SCREEN_HEIGHT - COMMAND_BLOCK_HEIGHT + 1 , L"명령 > " );
}

void HUD::RenderMainPanel(ScreenService& screen) const
{
	if ( false == m_layout.useMainTextPanel )
	{
		// 게임 화면 등으로 사용할 때는 HUD에서 패널을 덮어쓰지 않음.
		return;
	}

	const int32 startX = m_layout.mainPanelStartX + 1;
	const int32 startY = m_layout.mainPanelTopY;
	const int32 height = m_layout.mainPanelHeight;
	const int32 messageAreaWidth = max( 0 , SCREEN_WIDTH - m_layout.mainPanelStartX - m_layout.rightMargin );
	const wstring clearLine( static_cast<size_t>( messageAreaWidth ) , L' ' );

	for ( int32 offsetY = 0; offsetY < height; ++offsetY )
	{
		screen.Draw( startX , startY + offsetY , clearLine );
	}

	int32 outputY = startY;
	for (size_t i = 0; i < m_mainTextQueue.size(); ++i)
	{
		screen.Draw(startX, outputY, m_mainTextQueue[i]);
		++outputY;
	}
}

void HUD::RenderBottomPanel(ScreenService& screen) const
{
	//screen.Draw(9, SCREEN_HEIGHT - COMMAND_BLOCK_HEIGHT + 1, GAME_MASTER->GetInputService().GetInputBuffer());

	const int32 promptStartX = 9;
	const int32 maxLength = max( 0 , SCREEN_WIDTH - m_layout.rightMargin - promptStartX - 1 );

	wstring displayText = m_commandLineText.substr( 0 , static_cast<size_t>( maxLength ) );

	screen.Draw( promptStartX , SCREEN_HEIGHT - COMMAND_BLOCK_HEIGHT + 1 , displayText );
}

void HUD::WriteLine(const wstring& line)
{
	if (m_mainTextQueue.size() >= MAX_LINES)
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
}

void HUD::ResetLayout()
{
	m_layout = m_defaultLayout;
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
