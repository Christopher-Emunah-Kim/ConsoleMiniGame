#include "TitleContent.h"
#include "../../Core/GameMaster.h"
#include "../../Service/InputService.h"

void TitleContent::LoadSnakeContent()
{
	GAME_MASTER->GetContentManager().ChangeContent(static_cast<int>(EContent::SNAKE));
}

void TitleContent::LoadDungeonContent()
{
	GAME_MASTER->GetContentManager().ChangeContent( static_cast<int>( EContent::DUNGEON ) );
}

void TitleContent::ExitGame()
{
	GAME_MASTER->GetHUD().WriteLine( L"게임을 종료합니다." );
	GAME_MASTER->RequestQuit();
}

void TitleContent::MoveToPreviousMenu()
{
	if ( m_menuItems.empty() )
	{
		return;
	}

	if ( m_selectedIndex == 0 )
	{
		m_selectedIndex = m_menuItems.size() - 1;
	}
	else
	{
		--m_selectedIndex;
	}

	RenderTitleScreen();
}

void TitleContent::MoveToNextMenu()
{
	if ( m_menuItems.empty() )
	{
		return;
	}

	m_selectedIndex = ( m_selectedIndex + 1 ) % m_menuItems.size();

	RenderTitleScreen();
}

void TitleContent::SelectMenuItem()
{
	if ( m_menuItems.empty() )
	{
		return;
	}

	const FMenuItem& selectedItem = m_menuItems[m_selectedIndex];

	if ( selectedItem.action )
	{
		selectedItem.action();
	}
}

void TitleContent::InitializeMenu()
{
	m_menuItems.clear();
	m_selectedIndex = 0;

	m_menuItems.push_back( { L"1. THE SNAKE", bind( &TitleContent::LoadSnakeContent, this ) } );
	m_menuItems.push_back( { L"2. DUNGEON", bind( &TitleContent::LoadDungeonContent, this ) } );
	m_menuItems.push_back( { L"2. 게임 종료", bind( &TitleContent::ExitGame, this ) } );
}

void TitleContent::BindInputActions()
{
	InputService& input = GAME_MASTER->GetInputService();

	//Input Mapping for Menu Navigation
	input.BindKeyAction(
		{
				{ static_cast<InputService::InputKeyCode>( 'W' ), bind( &TitleContent::MoveToPreviousMenu, this ) },
				{ static_cast<InputService::InputKeyCode>( 'A' ), bind( &TitleContent::MoveToPreviousMenu, this ) },
				{ static_cast<InputService::InputKeyCode>( VK_UP ), bind( &TitleContent::MoveToPreviousMenu, this ) },
				{ static_cast<InputService::InputKeyCode>( VK_LEFT ), bind( &TitleContent::MoveToPreviousMenu, this ) },
				{ static_cast<InputService::InputKeyCode>( 'S' ), bind( &TitleContent::MoveToNextMenu, this ) },
				{ static_cast<InputService::InputKeyCode>( 'D' ), bind( &TitleContent::MoveToNextMenu, this ) },
				{ static_cast<InputService::InputKeyCode>( VK_DOWN ), bind( &TitleContent::MoveToNextMenu, this ) },
				{ static_cast<InputService::InputKeyCode>( VK_RIGHT ), bind( &TitleContent::MoveToNextMenu, this ) },
				{ static_cast<InputService::InputKeyCode>( VK_RETURN ), bind( &TitleContent::SelectMenuItem, this ) },
		}
		);
}

void TitleContent::RenderTitleScreen()
{
	HUD& hud = GAME_MASTER->GetHUD();
	hud.ClearText();

	hud.SetCommandLineText( L"WASD/방향키 : 이동 | Enter : 선택" );

	hud.WriteLine( L"■■■■■■■■■■■■■■■■■■■■■■" );
	hud.WriteLine( L"■                                         ■" );
	hud.WriteLine( L"■  ■■■  ■     ■   ■  ■■     ■    ■" );
	hud.WriteLine( L"■  ■   ■ ■     ■   ■ ■        ■    ■" );
	hud.WriteLine( L"■  ■   ■ ■     ■   ■ ■     ■■■■ ■" );
	hud.WriteLine( L"■  ■■■  ■     ■   ■  ■■     ■    ■" );
	hud.WriteLine( L"■  ■      ■     ■   ■      ■   ■    ■" );
	hud.WriteLine( L"■  ■      ■     ■   ■      ■   ■    ■" );
	hud.WriteLine( L"■  ■      ■■■   ■■   ■■     ■    ■" );
	hud.WriteLine( L"■                                         ■" );
	hud.WriteLine( L"■   C O N S O L E   G A M E   P A C K     ■" );
	hud.WriteLine( L"■             v e r . 0 . 3               ■" );
	hud.WriteLine( L"■■■■■■■■■■■■■■■■■■■■■■" );

	hud.WriteLine( L" " );
	hud.WriteLine( L" " );
	hud.WriteLine( L" " );
	hud.WriteLine( L"타이틀 화면입니다." );
	hud.WriteLine( L"WASD 혹은 방향키로 메뉴를 이동하고 Enter 키로 선택하세요." );
	hud.WriteLine( L" " );


	for ( size_t i = 0; i < m_menuItems.size(); ++i )
	{
		if ( i == m_selectedIndex )
		{
			hud.WriteLine( L"▶ " + m_menuItems[i].label + L"◀ ");
		}
		else
		{
			hud.WriteLine( L"  " + m_menuItems[i].label + L" ");
		}
	}

	hud.WriteLine( L" " );


}

void TitleContent::OnInit()
{
	HUD& hud = GAME_MASTER->GetHUD();

	FHudLayout layout = hud.GetDefaultLayout();
	layout.bEnableMainPanel = true;
	layout.bEnableBottomPanel = true;
	hud.SetLayout( layout );

	InitializeMenu();
	BindInputActions();
	RenderTitleScreen();
}

void TitleContent::OnRelease()
{
	m_menuItems.clear();
}

void TitleContent::OnUpdate()
{
	//No periodic updates needed for title screen
}

void TitleContent::OnRender()
{
	//Rendering is handled by HUD, so no additional rendering logic is needed here
}


