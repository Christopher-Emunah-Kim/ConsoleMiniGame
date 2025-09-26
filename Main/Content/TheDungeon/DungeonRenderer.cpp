#include "DungeonRenderer.h"
#include "DungeonGameState.h"
#include "../../../Service/ScreenService.h"
#include "../DefaultTypes.h"
#include "DungeonContentType.h"


void DungeonRenderer::Render( ScreenService& screen , const DungeonGameState& gameState ) const
{
	RenderGame( screen , gameState );
	RenderUI( screen , gameState );
}


void DungeonRenderer::RenderGame( ScreenService& screen , const DungeonGameState& gameState ) const
{
	const auto& map = gameState.GetDungeonMap();
	const FCoord playerPos = gameState.GetPlayer().GetCharacterInfo().position;

	const auto TileToConsoleX = []( int32 tileX )
		{
			// 던전 타일은 전각문자(2칸 폭)로 그리므로 X 좌표도 2배로 환산한다.
			return tileX * 2 + DUNGEON_LEFT_MARGIN;
		};

	for ( size_t y = 0; y < map.size(); ++y )
	{
		for ( size_t x = 0; x < map[ y ].size(); ++x )
		{
			// 타일 문자와 색상을 한 번에 결정한다.
			wchar_t tileChar = L' ';
			int8 fontColor = WHITE;

			switch ( static_cast<Cell>( map[ y ][ x ] ) )
			{
			case PATH:
				tileChar = L'　';
				break; // 전각 공백으로 통로 폭 유지
			case WALL:
				tileChar = L'■';
				fontColor = DARKGRAY;
				break;
			case START:
				tileChar = L'Ｓ';
				fontColor = SKYBLUE;
				break;
			case MONSTER:
				tileChar = L'！';
				fontColor = RED;
				break;
			case ITEM:
				tileChar = L'？';
				fontColor = YELLOW;
				break;
			case EXIT:
				tileChar = L'★';
				fontColor = GREEN;
				break;
			default:
				tileChar = L' ';
				break;
			}

			const wstring tileStr( 1 , tileChar );
			screen.Draw( TileToConsoleX( static_cast<int32>( x ) ) ,
				BOARD_TOP_MARGIN + static_cast<int32>( y ) ,
				tileStr , fontColor );
		}
	}

	screen.Draw( TileToConsoleX( playerPos.x ) , BOARD_TOP_MARGIN + playerPos.y , L"●" );
}

void DungeonRenderer::RenderUI( ScreenService& screen , const DungeonGameState& gameState ) const
{
	const DungeonPlayer& player = gameState.GetPlayer();
	const FCharacterInfo& info = player.GetCharacterInfo();

	const wstring hpText = L"HP : " + std::to_wstring( player.GetCurrentHP() ) + L" / " + std::to_wstring( info.maxHP );
	const wstring posText = L"위치 : (" + std::to_wstring( info.position.x ) + L", " + std::to_wstring( info.position.y ) + L")";
	
	screen.Draw( DUNGEON_LEFT_MARGIN , 2, hpText );
	screen.Draw( DUNGEON_LEFT_MARGIN , 3 , posText );

	const wstring& status = gameState.GetStatusMessage();

	if ( !status.empty() )
	{
		screen.Draw( DUNGEON_LEFT_MARGIN , 4 , status );
	}

	if ( gameState.IsGameOver() )
	{
		const wstring resultText = gameState.HasPlayerEscaped()
			? L"던전 탈출 성공! R 키로 재도전, Q 키로 종료."
			: L"전투에서 패배했습니다. R 키로 재도전하세요.";
		screen.Draw( DUNGEON_LEFT_MARGIN , 5 , resultText );
	}
}

