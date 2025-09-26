#include "SnakeContent.h"
#include "../../../Core/GameMaster.h"
#include "../../../Service/InputService.h"
#include "../../../Service/ScreenService.h"
#include "../../../Main/HUD.h"

namespace
{
	constexpr int32 DEFAULT_WIDTH = 40;
	constexpr int32 DEFAULT_HEIGHT = 20;
	constexpr float DEFAULT_MOVE_INTERVAL = 0.15f;
}

void SnakeContent::BindInput()
{
	InputService& input = GAME_MASTER->GetInputService();
	SnakeGameState& gameState = m_gameState;

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
		{ static_cast<InputService::InputKeyCode>( 'Q' ), [this]() { RequestExitToTitle(); } },
		{ static_cast<InputService::InputKeyCode>( VK_ESCAPE ), [this]() { RequestExitToTitle(); } },
		} );
}

void SnakeContent::ResetGame()
{
	m_bRequestExitToTitle = false;
	m_gameState.Reset();
}

void SnakeContent::RequestExitToTitle()
{
	m_bRequestExitToTitle = true;
}

void SnakeContent::OnInit()
{
	HUD& hud = GAME_MASTER->GetHUD();
	hud.ClearText();

	FHudLayout layout = hud.GetDefaultLayout();

	layout.bEnableMainPanel = false;
	layout.bEnableBottomPanel = true;

	hud.SetLayout( layout );
	hud.SetCommandLineText( L"WASD/방향키 : 이동 | R :  재시작 | Esc : 타이틀로" );

	m_gameState.Initialize( DEFAULT_WIDTH , DEFAULT_HEIGHT , DEFAULT_MOVE_INTERVAL );

	ResetGame();
	BindInput();
}

void SnakeContent::OnRelease()
{
	HUD& hud = GAME_MASTER->GetHUD();
	hud.ResetLayout();
	hud.ClearText();
	hud.SetCommandLineText( L"" );

	m_gameState.Clear();
	m_bRequestExitToTitle = false;
}

void SnakeContent::OnUpdate()
{
	if ( m_bRequestExitToTitle )
	{
		GAME_MASTER->GetContentManager().ChangeContent(static_cast<int>(EContent::TITLE));
		return;
	}

	if(m_gameState.IsGameOver())
	{
		return;
	}

	m_gameState.TryMove();
}

void SnakeContent::OnRender()
{
	ScreenService& screen = GAME_MASTER->GetScreen();
	m_renderer.Render( screen , m_gameState );
}
