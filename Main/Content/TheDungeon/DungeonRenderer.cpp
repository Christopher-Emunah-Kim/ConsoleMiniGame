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

	for ( size_t y = 0; y < map.size(); ++y )
	{
		wstring line;
		line.reserve( map[ y ].size() );

		for ( size_t x = 0; x < map[ y ].size(); ++x )
		{
			wchar_t tileChar = L' ';
			switch ( static_cast<Cell>( map[ y ][ x ] ) )
			{
			case WALL:    tileChar = L'■'; break;
			case START:   tileChar = L'S'; break;
			case MONSTER: tileChar = L'!'; break;
			case ITEM:    tileChar = L'?'; break;
			case EXIT:    tileChar = L'★'; break;
			default:      tileChar = L' '; break;
			}

			line.push_back( tileChar );
		}

		screen.Draw( 0 , BOARD_TOP_MARGIN + static_cast<int32>( y ) , line );
	}

	screen.Draw( playerPos.x , BOARD_TOP_MARGIN + playerPos.y , L"●" );
}

void DungeonRenderer::RenderUI( ScreenService& screen , const DungeonGameState& gameState ) const
{
	const DungeonPlayer& player = gameState.GetPlayer();
	const FCharacterInfo& info = player.GetCharacterInfo();

	const wstring hpText = L"HP : " + std::to_wstring( player.GetCurrentHP() ) + L" / " + std::to_wstring( info.maxHP );
	screen.Draw( 0 , 0 , hpText );

	const wstring posText = L"위치 : (" + std::to_wstring( info.position.x ) + L", " + std::to_wstring( info.position.y ) + L")";
	screen.Draw( 0 , 1 , posText );

	const wstring& status = gameState.GetStatusMessage();
	if ( !status.empty() )
	{
		screen.Draw( 0 , 3 , status );
	}

	if ( gameState.IsGameOver() )
	{
		const wstring resultText = gameState.HasPlayerEscaped()
			? L"던전 탈출 성공! R 키로 재도전, Q 키로 종료."
			: L"전투에서 패배했습니다. R 키로 재도전하세요.";
		screen.Draw( 0 , 4 , resultText );
	}
}

