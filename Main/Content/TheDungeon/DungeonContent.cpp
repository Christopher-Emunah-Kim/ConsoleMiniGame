#include "DungeonContent.h"
#include "../../../Core/GameMaster.h"
#include "../../../Service/InputService.h"
#include "../../../Service/ScreenService.h"
#include "../../../Main/HUD.h"

void DungeonContent::BindInput()
{
	InputService& input = GAME_MASTER->GetInputService();

	input.BindKeyAction( {
		{ static_cast<InputService::InputKeyCode>( 'W' ), [this]() { m_gameState.EnqueueDirection( EDirection::UP ); } },
		{ static_cast<InputService::InputKeyCode>( 'A' ), [this]() { m_gameState.EnqueueDirection( EDirection::LEFT ); } },
		{ static_cast<InputService::InputKeyCode>( 'S' ), [this]() { m_gameState.EnqueueDirection( EDirection::DOWN ); } },
		{ static_cast<InputService::InputKeyCode>( 'D' ), [this]() { m_gameState.EnqueueDirection( EDirection::RIGHT ); } },
		{ static_cast<InputService::InputKeyCode>( VK_UP ), [this]() { m_gameState.EnqueueDirection( EDirection::UP ); } },
		{ static_cast<InputService::InputKeyCode>( VK_LEFT ), [this]() {m_gameState.EnqueueDirection( EDirection::LEFT ); } },
		{ static_cast<InputService::InputKeyCode>( VK_DOWN ), [this]() { m_gameState.EnqueueDirection( EDirection::DOWN ); } },
		{ static_cast<InputService::InputKeyCode>( VK_RIGHT ), [this]() { m_gameState.EnqueueDirection( EDirection::RIGHT ); } },
		{ static_cast<InputService::InputKeyCode>( 'R' ), [this]() { ResetGame(); } },
		{ static_cast<InputService::InputKeyCode>( 'Q' ), [this]() { ReqeustExitToTitle(); } },
		{ static_cast<InputService::InputKeyCode>( VK_ESCAPE ), [this]() { ReqeustExitToTitle(); } },
		} );
}

void DungeonContent::ResetGame()
{
	m_bRequestExitToTitle = false;
	m_gameState.Reset();
}

void DungeonContent::ReqeustExitToTitle()
{
	m_bRequestExitToTitle = true;
}

void DungeonContent::OnInit()
{
	HUD& hud = GAME_MASTER->GetHUD();
	hud.ClearText();

	FHudLayout layout = hud.GetDefaultLayout();

	layout.bEnableMainPanel = false;
	layout.bEnableBottomPanel = true;

	hud.SetLayout( layout );
	hud.SetCommandLineText( L"WASD/방향키 : 이동 | R :  재시작 | Esc : 타이틀로" );

	m_gameState.Initialize( DUNGEON_AREA_WIDTH , DUNGEON_AREA_HEIGHT );

	ResetGame();

	BindInput();
}

void DungeonContent::OnRelease()
{
	HUD& hud = GAME_MASTER->GetHUD();
	hud.ResetLayout();
	hud.ClearText();
	hud.SetCommandLineText( L"" );

	m_gameState.Clear();
}

void DungeonContent::OnUpdate()
{
	if ( m_bRequestExitToTitle )
	{
		GAME_MASTER->GetContentManager().ChangeContent( static_cast<int>( EContent::TITLE ) );
		return;
	}

	if ( m_gameState.IsGameOver() )
	{
		return;
	}

	m_gameState.TryPlayerMove();
}

void DungeonContent::OnRender()
{
	ScreenService& screen = GAME_MASTER->GetScreen();
	m_renderer.Render( screen , m_gameState );
}
