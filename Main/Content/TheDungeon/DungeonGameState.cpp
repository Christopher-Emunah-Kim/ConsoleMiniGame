#include "DungeonGameState.h"
#include <random>

void DungeonGameState::UpdatePlayerPosition()
{
	m_currentDirection = m_nextDirection;

	FCoord& pos = m_player.GetCharacterInfo().position;

	switch ( m_currentDirection )
	{
		case EDirection::UP:
			pos.y -= 1;
			break;
		case EDirection::DOWN:
			pos.y += 1;
			break;
		case EDirection::LEFT:
			pos.x -= 1;
			break;
		case EDirection::RIGHT:
			pos.x += 1;
			break;

		default: 
			m_currentDirection = EDirection::NONE;
			break;
	}

	if(pos.x < 0 || pos.y < 0 )
	{
		return;
	}

}

vector<vector<int>> DungeonGameState::GenerateDungeonMap( const FDungeonGeneratingParams& params )
{
	std::mt19937 rng( std::random_device{}( ) );
	std::uniform_real_distribution<double> coin( 0.0 , 1.0 );    

	const int width = params.width;
	const int height = params.height;
	const int startX = 1 , startY = 1;                   //START
	const int exitX = width - 2 , exitY = height - 2;    //EXIT

	std::vector<std::vector<int>> grid( height , std::vector<int>( width , WALL ) );

	// START/EXIT 제외 내부 채우기
	for ( int y = 1; y < height - 1; ++y )
	{
		for ( int x = 1; x < width - 1; ++x )
		{
			if ( ( x == startX && y == startY ) || ( x == exitX && y == exitY ) )
				continue;

			// 난수 < wallDensity면 벽, 아니면 통로
			if ( coin( rng ) < params.wallDensity )
			{
				grid[ y ][ x ] = WALL;
			}
			else
			{
				grid[ y ][ x ] = PATH;
			}
		}
	}

	grid[ startX ][ startY ] = START;
	grid[ exitY ][ exitX ] = EXIT;

	//통과보장 L자 경로 파기
	CarveLPath( grid );
	

	// 통로 위에 몬스터/아이템 확률 배치
	// 시작/출구는 제외, 벽에는 배치하지 않음
	for ( int y = 1; y < height - 1; ++y )
	{
		for ( int x = 1; x < width - 1; ++x )
		{
			if ( ( x == startX && y == startY ) || ( x == exitX && y == exitY ) )
				continue;

			if ( grid[ y ][ x ] == PATH )
			{
				// r < monster_prob → 몬스터
				// monster_prob ≤ r < monster_prob + item_prob → 아이템
				double random = coin( rng );
				if ( random < params.monsterProb )
				{
					grid[ y ][ x ] = MONSTER;
				}
				else if ( random < (params.monsterProb + params.itemProb) )
				{
					grid[ y ][ x ] = ITEM;
				}
			}
		}
	}

	return grid;
}

// L자 형태로 시작↔출구 경로 파기 ( 미로	통과 보장)
void DungeonGameState::CarveLPath( vector<vector<int>>& grid )
{
	const int height = static_cast<int>( grid.size() );
	const int width = static_cast<int>( grid[ 0 ].size() );
	const int startX = 1;
	const int startY = 1;
	const int exitX = width - 2;
	const int exitY = height - 2;

	int x = startX;
	int y = startY;

	while ( x != exitX )
	{
		if ( grid[ startY ][ x ] == WALL )
		{
			grid[ startY ][ x ] = PATH;
		}
		
		if(exitX > x)
		{
			++x;
		}
		else if(exitX < x)
		{
			--x;
		}
	}

	while ( y != exitY )
	{
		if ( grid[ y ][ exitX ] == WALL )
		{
			grid[ y ][ exitX ] = PATH;
		}
		if ( exitY > y )
		{
			++y;
		}
		else if ( exitY < y )
		{
			--y;
		}
	}

	grid[ startX ][ startY ] = START;
	grid[ exitX ][ exitY ] = EXIT;
}

void DungeonGameState::Initialize( int32 areaWidth , int32 areaHeight )
{
	// 0 : 통로 / 1 : 벽 / 2 : 플레이어 시작위치 / 3 : 몬스터 / 4: 아이템 / 5. 출구
	// DUNGEON_AREA_WIDTH/HEIGHT에 맞게 던전 맵 배열 생성
	// ㄴ 가장 바깥쪽 테두리는 모두 벽(1)로 설정
	// ㄴ 플레이어 시작위치(2)는 (1,1)로 설정
	// ㄴ 출구(5)는 (DUNGEON_AREA_WIDTH-2, DUNGEON_AREA_HEIGHT-2)로 설정
	// ㄴ 나머지 공간은 랜덤하게 벽(1)과 통로(0)로 채움 (단, 플레이어 시작위치와 출구는 제외)
	// ㄴ 몬스터(3)와 아이템(4)는 일정 확률로 랜덤하게 배치 (단, 플레이어 시작위치와 출구는 제외)
	m_player.GetCharacterInfo().position = { 1, 1 };
	m_currentDirection = EDirection::NONE;
	m_nextDirection = EDirection::NONE;
	m_bIsGameOver = false;

	//던전 맵 생성 로직
	FDungeonGeneratingParams genParams;
	genParams.width = areaWidth;
	genParams.height = areaHeight;

	GenerateDungeonMap( genParams );

	m_player.Init();

}

void DungeonGameState::Reset()
{
	m_bIsGameOver = false;
	m_currentDirection = EDirection::NONE;

	Initialize( DUNGEON_AREA_WIDTH , DUNGEON_AREA_HEIGHT );
}

void DungeonGameState::Clear()
{
	m_bIsGameOver = false;
	m_currentDirection = EDirection::NONE;
}

bool DungeonGameState::TryPlayerMove( )
{
	if(m_bIsGameOver)
	{
		return false;
	}

	UpdatePlayerPosition();
	return true;
}

void DungeonGameState::EnqueueDirection( EDirection dir )
{
	if ( dir == EDirection::NONE )
	{
		return;
	}
	// Prevent reversing direction
	if ( ( m_currentDirection == EDirection::UP && dir == EDirection::DOWN ) ||
		 ( m_currentDirection == EDirection::DOWN && dir == EDirection::UP ) ||
		 ( m_currentDirection == EDirection::LEFT && dir == EDirection::RIGHT ) ||
		 ( m_currentDirection == EDirection::RIGHT && dir == EDirection::LEFT ) )
	{
		return;
	}
	m_nextDirection = dir;
}
