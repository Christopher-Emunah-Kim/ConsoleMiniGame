#include "SnakeRenderer.h"
#include "SnakeGameState.h"
#include "../../../Core/GameMaster.h"
#include "../../../Service/ScreenService.h"
#include "../../../Main/HUD.h"


void SnakeRenderer::Render( ScreenService& screen , const SnakeGameState& gameState ) const
{
	RenderGame( screen , gameState );
	RenderUI( screen , gameState );
}

void SnakeRenderer::RenderGame( ScreenService& screen , const SnakeGameState& gameState ) const
{
	HUD& hud = GAME_MASTER->GetHUD();

	const FHudViewport viewport = hud.GetGameViewportRect();
	const int32 blankWidth = max( 0 , viewport.width );
	const wstring blankLine( static_cast<size_t>( blankWidth ) , L' ' );

	for ( int32 offsetY = 0; offsetY < gameState.GetHeight(); ++offsetY )
	{
		screen.Draw( viewport.x , viewport.y + offsetY , blankLine );
	}

	// Draw Borders
	const int32 boardPixelWidth = gameState.GetWidth() * CELL_WIDTH;
	const int32 boardTop = viewport.y + BOARD_TOP_MARGIN;
	const int32 boardBottom = viewport.y + viewport.height;
	const int32 boardLeft = viewport.x + max( 0 , ( viewport.width - boardPixelWidth ) / 2 );
	const int32 boardRight = boardLeft + boardPixelWidth;

	if ( boardTop >= boardBottom )
	{
		return;
	}

	for ( int32 x = 0; x < gameState.GetWidth(); ++x )
	{
		const int32 drawX = boardLeft + x * CELL_WIDTH;
		const int32 topY = boardTop - 1;
		const int32 bottomY = boardBottom + gameState.GetHeight();

		if ( topY >= viewport.y )
		{
			screen.Draw( drawX , topY , L"■" , GREEN );
		}

		if ( bottomY < boardBottom )
		{
			screen.Draw( drawX , bottomY , L"■" , GREEN );
		}
	}

	for ( int32 y = 0; y < gameState.GetHeight(); ++y )
	{
		const int32 drawY = boardTop + y;
		const int32 leftX = boardLeft - CELL_WIDTH;
		const int32 rightX = boardLeft + gameState.GetWidth() * CELL_WIDTH;

		if ( leftX >= viewport.x )
		{
			screen.Draw( leftX , drawY , L"■" , GREEN );
		}
		if ( rightX < viewport.x + viewport.width )
		{
			screen.Draw( rightX , drawY , L"■" , GREEN );
		}
	}

	for ( size_t idx = 0; idx < gameState.GetSnake().size(); ++idx )
	{
		const FCoord& segment = gameState.GetSnake()[ idx ];

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

	if ( gameState.HasFood() )
	{
		const int32 drawX = boardLeft + gameState.GetFood().x * CELL_WIDTH;
		const int32 drawY = boardTop + gameState.GetFood().y;

		if ( drawX >= viewport.x && drawX < boardRight && drawY >= boardTop && drawY < boardBottom )
		{
			screen.Draw( drawX , drawY , L"★" , BLUE );
		}
	}
}

void SnakeRenderer::RenderUI( ScreenService& screen , const SnakeGameState& gameState ) const
{
	HUD& hud = GAME_MASTER->GetHUD();

	const FHudViewport viewport = hud.GetGameViewportRect();

	const int32 infoX = 4;
	const int32 infoY = viewport.y;

	for ( int16 y = 1; y < SCREEN_HEIGHT - 3; ++y )
	{
		screen.Draw( viewport.x , y , L"│" );
	}


	screen.Draw( infoX , infoY + 1 , L"=== S N A K E ===" , YELLOW );
	screen.Draw( infoX , infoY + 3 , L"점수 : " + to_wstring( gameState.GetScore() ) , SKYBLUE );
	screen.Draw( infoX , infoY + 5 , L"길이 : " + to_wstring( gameState.GetSnake().size()) , SKYBLUE);

	if ( gameState.IsGameOver() )
	{
		screen.Draw( infoX , infoY + 7 , L"게임 오버!" , RED );
		screen.Draw( infoX , infoY + 8 , L"R : 재시작" , DARKYELLOW );
		screen.Draw( infoX , infoY + 9 , L"ESC 키로 타이틀로" , DARKYELLOW );
	}
	else
	{
		screen.Draw( infoX , infoY + 7 , L"상태 : 진행 중" , GREEN );
		if ( gameState.HasFood() )
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
