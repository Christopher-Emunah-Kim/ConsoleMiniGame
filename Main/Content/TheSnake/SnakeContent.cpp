#include "SnakeContent.h"
#include "../../../Core/GameMaster.h"
#include "../../../Service/InputService.h"
#include "../../../Service/ScreenService.h"
#include "../../../Main/HUD.h"

void SnakeContent::BindInput()
{
	InputService& input = GAME_MASTER->GetInputService();

	input.BindKeyAction( {
		{ static_cast<InputService::InputKeyCode>( 'W' ), [this]() { EnqueueDirection( EDirection::UP ); } },
		{ static_cast<InputService::InputKeyCode>( 'A' ), [this]() { EnqueueDirection( EDirection::LEFT ); } },
		{ static_cast<InputService::InputKeyCode>( 'S' ), [this]() { EnqueueDirection( EDirection::DOWN ); } },
		{ static_cast<InputService::InputKeyCode>( 'D' ), [this]() { EnqueueDirection( EDirection::RIGHT ); } },
		{ static_cast<InputService::InputKeyCode>( VK_UP ), [this]() { EnqueueDirection( EDirection::UP ); } },
		{ static_cast<InputService::InputKeyCode>( VK_LEFT ), [this]() { EnqueueDirection( EDirection::LEFT ); } },
		{ static_cast<InputService::InputKeyCode>( VK_DOWN ), [this]() { EnqueueDirection( EDirection::DOWN ); } },
		{ static_cast<InputService::InputKeyCode>( VK_RIGHT ), [this]() { EnqueueDirection( EDirection::RIGHT ); } },
		{ static_cast<InputService::InputKeyCode>( 'R' ), [this]() { ResetGame(); } },
		{ static_cast<InputService::InputKeyCode>( 'Q' ), [this]() { RequestExitToTitle(); } },
		{ static_cast<InputService::InputKeyCode>( VK_ESCAPE ), [this]() { RequestExitToTitle(); } },
		} );
}

void SnakeContent::ResetGame()
{
	m_snake.clear();
	m_snake.push_back({ m_gameAreaWidth /2, m_gameAreaHeight / 2});
	m_snake.push_back({ m_gameAreaWidth /2 - 1, m_gameAreaHeight / 2});
	m_snake.push_back({ m_gameAreaWidth /2 - 2, m_gameAreaHeight / 2});


	m_currentDirection = EDirection::RIGHT;
	m_nextDirection = EDirection::RIGHT;

	m_score = 0;
	m_bHasFood = false;
	m_bIsGameOver = false;
	m_bRequestExitToTitle = false;

	m_lastMoveTime = steady_clock::now();

	SpawnFood();
}

void SnakeContent::SpawnFood()
{
	if ( m_snake.empty() )
	{
		return;
	}

	const int32 maxCells = m_gameAreaWidth * m_gameAreaHeight;
	if(static_cast<int32>(m_snake.size() ) >= maxCells )
	{
		// No space to spawn food
		m_bHasFood = false;
		return;
	}

	for(int32 tryCount = 0; tryCount < maxCells; ++tryCount)
	{
		const int32 x = rand() % m_gameAreaWidth;
		const int32 y = rand() % m_gameAreaHeight;

		bool overlap = false;

		for(const FCoord& segment : m_snake)
		{
			if(segment.x == x && segment.y == y)
			{
				overlap = true;
				break;
			}
		}

		if ( !overlap )
		{
			m_food = { x , y };
			m_bHasFood = true;
			return;
		}
	}

	m_bHasFood = false;
}

void SnakeContent::UpdateSnakePosition()
{
	if(m_snake.empty() || m_bIsGameOver)
	{
		return;
	}

	m_currentDirection = m_nextDirection;
	FCoord head = m_snake.front();

	switch ( m_currentDirection )
	{
	case EDirection::UP: 
		head.y -= 1; 
		break;
	case EDirection::DOWN: 
		head.y += 1; 
		break;
	case EDirection::LEFT: 
		head.x -= 1; 
		break;
	case EDirection::RIGHT: 
		head.x += 1; 
		break;
	default: break;
	}

	if(head.x < 0 || head.x >= m_gameAreaWidth || head.y < 0 || head.y >= m_gameAreaHeight)
	{
		m_bIsGameOver = true;
		return;
	}

	if(IsCollision(head.x, head.y))
	{
		m_bIsGameOver = true;
		return;
	}

	m_snake.insert( m_snake.begin() , head );

	if(m_bHasFood && head.x == m_food.x && head.y == m_food.y)
	{
		m_score += 10;
		m_bHasFood = false;

		SpawnFood();
	}
	else
	{
		m_snake.pop_back();
	}
}

void SnakeContent::RenderGame( ScreenService& screen )
{
	HUD& hud = GAME_MASTER->GetHUD();

	const FHudViewport viewport = hud.GetGameViewportRect();
	const int32 blankWidth = max( 0 , viewport.width );
	const wstring blankLine( static_cast<size_t>( blankWidth ) , L' ' );

	for(int32 offsetY = 0; offsetY < m_gameAreaHeight; ++offsetY)
	{
		screen.Draw( viewport.x , viewport.y + offsetY , blankLine );
	}

	// Draw Borders
	const int32 boardPixelWidth = m_gameAreaWidth * CELL_WIDTH;
	const int32 boardTop = viewport.y + BOARD_TOP_MARGIN;
	const int32 boardBottom = viewport.y + viewport.height;
	const int32 boardLeft = viewport.x + max(0, ( viewport.width - boardPixelWidth ) / 2);
	const int32 boardRight = boardLeft + boardPixelWidth;

	if ( boardTop >= boardBottom )
	{
		return;
	}

	for ( int32 x = 0; x < m_gameAreaWidth; ++x )
	{
		const int32 drawX = boardLeft + x * CELL_WIDTH;
		const int32 topY = boardTop - 1;
		const int32 bottomY = boardBottom + m_gameAreaHeight;

		if ( topY >= viewport.y )
		{
			screen.Draw( drawX , topY , L"■" , GREEN );
		}

		if ( bottomY < boardBottom )
		{
			screen.Draw( drawX , bottomY , L"■" , GREEN );
		}
	}

	for(int32 y = 0; y < m_gameAreaHeight; ++y )
	{
		const int32 drawY = boardTop + y;
		const int32 leftX = boardLeft - CELL_WIDTH;
		const int32 rightX = boardLeft + m_gameAreaWidth * CELL_WIDTH;

		if ( leftX >= viewport.x )
		{
			screen.Draw( leftX , drawY , L"■" , GREEN );
		}
		if ( rightX < viewport.x + viewport.width )
		{
			screen.Draw( rightX , drawY , L"■" , GREEN );
		}
	}

	for(size_t idx = 0; idx < m_snake.size(); ++idx)
	{
		const FCoord& segment = m_snake[idx];

		const int32 drawX = boardLeft + segment.x * CELL_WIDTH;
		const int32 drawY = boardTop + segment.y;

		if ( drawX < viewport.x || drawX >= boardRight || drawY < boardTop || drawY >= boardBottom )
		{
			continue;
		}

		if ( idx == 0 )
		{
			screen.Draw( drawX , drawY , L"●" , SKYBLUE );
		}
		else
		{
			screen.Draw( drawX , drawY , L"■" , DARKSKYBLUE );
		}
	}

	if ( m_bHasFood )
	{
		const int32 drawX = boardLeft + m_food.x * CELL_WIDTH;
		const int32 drawY = boardTop + m_food.y;

		if ( drawX >= viewport.x && drawX < boardRight && drawY >= boardTop && drawY < boardBottom )
		{
			screen.Draw( drawX , drawY , L"★" , BLUE );
		}
	}
}

void SnakeContent::RenderUI( ScreenService& screen )
{
	HUD& hud = GAME_MASTER->GetHUD();

	const FHudViewport viewport = hud.GetGameViewportRect();

	const int32 infoX = 4;
	const int32 infoY = viewport.y;

	for ( int16 y = 1; y < SCREEN_HEIGHT - COMMAND_BLOCK_HEIGHT; ++y )
	{
		screen.Draw( viewport.x, y , L"│" );
	}


	screen.Draw( infoX , infoY + 1 , L"=== S N A K E ===" , YELLOW );
	screen.Draw( infoX , infoY + 3 , L"점수 : " + to_wstring( m_score ) , SKYBLUE );
	screen.Draw( infoX , infoY + 5 , L"길이 : " + to_wstring( m_snake.size() ) , SKYBLUE );

	if ( m_bIsGameOver )
	{
		screen.Draw( infoX , infoY + 7 , L"게임 오버!" , RED );
		screen.Draw( infoX , infoY + 8 , L"R : 재시작" , DARKYELLOW );
		screen.Draw( infoX , infoY + 9 , L"ESC 키로 타이틀로" , DARKYELLOW );
	}
	else
	{
		screen.Draw( infoX , infoY + 7 , L"상태 : 진행 중" , GREEN );
		if(m_bHasFood)
		{
			screen.Draw( infoX , infoY + 9 , L"음식이 나타났습니다!" , DARKGREEN );
			screen.Draw( infoX , infoY + 10 , L"먹으면 10점 획득!" , DARKGREEN );
		}
		else
		{
			screen.Draw( infoX , infoY + 9 , L"먹이 생성 대기중..." , DARKYELLOW );
		}
	}

}

void SnakeContent::EnqueueDirection( EDirection dir )
{
	if(dir == EDirection::NONE || m_bIsGameOver)
	{
		return;
	}

	if((dir == EDirection::UP && m_currentDirection == EDirection::DOWN) ||
		( dir == EDirection::DOWN && m_currentDirection == EDirection::UP ) ||
		( dir == EDirection::LEFT && m_currentDirection == EDirection::RIGHT ) ||
		( dir == EDirection::RIGHT && m_currentDirection == EDirection::LEFT ))
	{
		return;
	}

	m_nextDirection = dir;
}

bool SnakeContent::IsCollision( int32 x , int32 y ) const
{
	for(const FCoord& segment : m_snake)
	{
		if(segment.x == x && segment.y == y)
		{
			return true;
		}
	}


	return false;
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

	layout.infoPanelWidth = 24;
	layout.mainPanelStartX = layout.infoPanelWidth + 2;
	layout.useMainTextPanel = false;

	hud.SetLayout( layout );
	hud.SetCommandLineText( L"WASD/방향키 : 이동 | R :  재시작 | Esc : 타이틀로" );

	BindInput();
	ResetGame();
}

void SnakeContent::OnRelease()
{
	HUD& hud = GAME_MASTER->GetHUD();
	hud.ResetLayout();
	hud.ClearText();
	hud.SetCommandLineText( L"" );

	m_snake.clear();
	m_bHasFood = false;
	m_bIsGameOver = false;
	m_bRequestExitToTitle = false;
}

void SnakeContent::OnUpdate()
{
	if ( m_bRequestExitToTitle )
	{
		GAME_MASTER->GetContentManager().ChangeContent( static_cast<int>( EContent::TITLE ) );
		return;
	}
	if ( m_bIsGameOver )
	{
		return;
	}

	const steady_clock::time_point now = steady_clock::now();
	const duration<float> deltaTime = now - m_lastMoveTime;

	if(deltaTime.count() < m_moveInterval)
	{
		return;
	}

	m_lastMoveTime = now;
	UpdateSnakePosition();
}

void SnakeContent::OnRender()
{
	ScreenService& screen = GAME_MASTER->GetScreen();
	RenderGame( screen );
	RenderUI( screen );
}
