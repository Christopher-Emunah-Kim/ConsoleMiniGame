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
	
	const int32 boardWidth = gameState.GetWidth();
	const int32 boardHeight = gameState.GetHeight();

	const int32 boardPixelWidth = boardWidth * CELL_WIDTH;
	const int32 boardTop = viewport.y + BOARD_TOP_MARGIN;
	const int32 boardBottom = boardTop + boardHeight;
	const int32 boardLeft = viewport.x + max( 0 , ( viewport.width - boardPixelWidth ) / 2 );
	const int32 boardRight = boardLeft + boardPixelWidth;


	/// Clear Viewport
	const int32 blankWidth = max( viewport.width , boardPixelWidth + CELL_WIDTH * 2);
	const int32 blankHeight = max( viewport.height , boardHeight + BOARD_TOP_MARGIN * 2 );
	const wstring blankLine( static_cast<size_t>( blankWidth ) , L' ' );

	for ( int32 offsetY = 0; offsetY < blankHeight; ++offsetY )
	{
		screen.Draw( viewport.x , viewport.y + offsetY , blankLine );
	}

	// Draw Borders
	const int32 topBorderY = boardTop - 1;
	const int32 bottomBorderY = boardBottom;

	//Top & Bottom Borders
	for ( int32 x = 0; x < boardWidth; ++x )
	{
		const int32 drawX = boardLeft + x * CELL_WIDTH;

		if ( topBorderY >= 0 )
		{
			screen.Draw( drawX , topBorderY , L"■" , GREEN );
		}

		if ( bottomBorderY < SCREEN_HEIGHT )
		{
			screen.Draw( drawX , bottomBorderY , L"■" , GREEN );
		}
	}

	//Left & Right Borders
	const int32 leftBorderX = boardLeft - CELL_WIDTH;
	const int32 rightBorderX = boardRight;

	for ( int32 y = 0; y < boardHeight; ++y )
	{
		const int32 drawY = boardTop + y;

		if ( leftBorderX >= 0 )
		{
			screen.Draw( leftBorderX , drawY , L"■" , GREEN );
		}
		if ( rightBorderX < SCREEN_WIDTH )
		{
			screen.Draw( rightBorderX , drawY , L"■" , GREEN );
		}
	}

	// Draw Snake & Food
	for ( size_t idx = 0; idx < gameState.GetSnake().size(); ++idx )
	{
		const FCoord& segment = gameState.GetSnake()[ idx ];

		const int32 drawX = boardLeft + segment.x * CELL_WIDTH;
		const int32 drawY = boardTop + segment.y;

		if ( drawX < boardLeft || drawX >= boardRight || drawY < boardTop || drawY >= boardBottom )
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

		if ( drawX >= boardLeft && drawX < boardRight && drawY >= boardTop && drawY < boardBottom )
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
