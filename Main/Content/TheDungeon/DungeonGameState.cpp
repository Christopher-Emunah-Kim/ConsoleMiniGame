#include "DungeonGameState.h"
#include <random>

bool DungeonGameState::UpdatePlayerPosition()
{
	if(m_dungeonMap.empty())
	{
		return false;
	}

	if(m_nextDirection == EDirection::NONE)
	{
		return false;
	}

	const FCoord currentPos = m_player.GetCharacterInfo().position;
	FCoord nextPos = currentPos;

	m_currentDirection = m_nextDirection;

	switch ( m_currentDirection )
	{
		case EDirection::UP:
			--nextPos.y;
			break;
		case EDirection::DOWN:
			++nextPos.y;
			break;
		case EDirection::LEFT:
			--nextPos.x;
			break;
		case EDirection::RIGHT:
			++nextPos.x;
			break;

		default: 
			m_currentDirection = EDirection::NONE;
			break;
	}

	if(!IsInBounds(nextPos))
	{
		m_statusMessage = L"경계를 벗어날 수 없습니다!";
		m_nextDirection = EDirection::NONE;
		return false;
	}

	const int8 cellType = static_cast<int8>( m_dungeonMap[ nextPos.y ][ nextPos.x ] );

	if ( cellType == WALL )
	{
		m_statusMessage = L"벽이 막고 있어 이동할 수 없습니다!";
		m_nextDirection = EDirection::NONE;
		return false;
	}

	
	m_player.GetCharacterInfo().position = nextPos;

	HandleCurrentCellEvent( nextPos , cellType );

	m_nextDirection = EDirection::NONE;

	return true;
}

vector<vector<int>> DungeonGameState::GenerateDungeonMap( const FDungeonGeneratingParams& params )
{
	// 0 : 통로 / 1 : 벽 / 2 : 플레이어 시작위치 / 3 : 몬스터 / 4: 아이템 / 5. 출구
	// DUNGEON_AREA_WIDTH/HEIGHT에 맞게 던전 맵 배열 생성
	// ㄴ 가장 바깥쪽 테두리는 모두 벽(1)로 설정
	// ㄴ 플레이어 시작위치(2)는 (1,1)로 설정
	// ㄴ 출구(5)는 (DUNGEON_AREA_WIDTH-2, DUNGEON_AREA_HEIGHT-2)로 설정
	// ㄴ 나머지 공간은 랜덤하게 벽(1)과 통로(0)로 채움 (단, 플레이어 시작위치와 출구는 제외)
	// ㄴ 몬스터(3)와 아이템(4)는 일정 확률로 랜덤하게 배치 (단, 플레이어 시작위치와 출구는 제외)
	std::mt19937 rng( std::random_device{}( ) );
	std::uniform_real_distribution<double> coin( 0.0 , 1.0 );    

	const int width = params.width;
	const int height = params.height;
	const int startX = 1;
	const int startY = 1;            //START
	const int exitX = width - 2;
	const int exitY = height - 2;    //EXIT

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

	grid[ startY ][ startX ] = START;
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

void DungeonGameState::ResetDungeon()
{
	FDungeonGeneratingParams genParams;
	genParams.width = m_dungeonWidth;
	genParams.height = m_dungeonHeight;

	m_dungeonMap = GenerateDungeonMap( genParams );
	m_player.GetCharacterInfo().position = { DEFAULT_PLAYER_X , DEFAULT_PLAYER_Y };
	m_player.Init();
	m_currentDirection = EDirection::NONE;
	m_nextDirection = EDirection::NONE;
	m_bIsGameOver = false;
	m_bPlayerEscaped = false;
	m_statusMessage = L"던전 탐험을 시작합니다.";
}

// L자 형태로 시작↔출구 경로 파기 ( 필수 통행 경로 확보 )
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

	grid[ startY ][ startX ] = START;
	grid[ exitY ][ exitX ] = EXIT;
}


void DungeonGameState::HandleCurrentCellEvent( const FCoord& nextPos , int8 cellType )
{
	switch ( cellType )
	{
	case START:
	case PATH:
		m_statusMessage = L"조심스레 복도를 살펴봅니다.";
		break;
	case MONSTER:
	{
		HandleMonsterEvent( nextPos );
	}
	break;
	case ITEM:
	{
		const int32 pervHP = m_player.GetCurrentHP();
		const int32 healAmount = 20;
		m_player.Heal( healAmount );
		m_statusMessage = L"체력을 " + to_wstring( m_player.GetCurrentHP() - pervHP ) + L" 회복했습니다!";
	}
	break;
	case EXIT:
	{
		m_bPlayerEscaped = true;
		m_statusMessage = L"무사히 던전을 탈출했습니다!";
	}
	break;

	default:
		m_statusMessage = L"알 수 없는 공간에 들어섰습니다...";
		break;
	}
}

void DungeonGameState::HandleMonsterEvent( const FCoord& nextPos )
{

	DungeonMonster* monster1 = new DungeonMonster( L"던전 고블린" );
	FCharacterInfo monsterInfo1 = { nextPos , L"던전 고블린" , 8 , 3 , 4 , 30 };
	monster1->GetCharacterInfo() = monsterInfo1;

	DungeonMonster* monster2 = new DungeonMonster ( L"던전 오크" );
	FCharacterInfo monsterInfo2 = { nextPos , L"던전 오크" , 12 , 6 , 2 , 50 };
	monster2->GetCharacterInfo() = monsterInfo2;

	DungeonMonster* monster3 = new DungeonMonster ( L"던전 트롤" );
	FCharacterInfo monsterInfo3 = { nextPos , L"던전 트롤" , 15 , 8 , 1 , 80 };
	monster3->GetCharacterInfo() = monsterInfo3;

	DungeonMonster* targetMonster;
	int8 randMonster = rand() % 3;
	
	switch ( randMonster )
	{
	case 0:
		targetMonster = monster1;
		break;
	case 1:
		targetMonster = monster2;
		break;
	case 2:
		targetMonster = monster3;
		break;

	default:
		targetMonster = monster1;
		break;
	}

	if ( targetMonster == nullptr )
	{
		m_statusMessage = L"몬스터가 나타나지 않았습니다.";
		return;
	}

	if ( ProcessBattle( targetMonster ) )
	{
		m_dungeonMap[ nextPos.y ][ nextPos.x ] = PATH;
		m_statusMessage = targetMonster->GetCharacterInfo().name + L"를 물리쳤습니다!";
	}
	else
	{
		m_bIsGameOver = true;
		m_statusMessage = targetMonster->GetCharacterInfo().name + L"에게 패배했습니다...";
	}

	delete monster1;
	delete monster2;
	delete monster3;
}

bool DungeonGameState::ProcessBattle( DungeonMonster* monster )
{
	monster->Init();

	bool bPlayerTurn = m_player.GetCharacterInfo().agility >= monster->GetCharacterInfo().agility;

	while(m_player.IsAlive() && monster->IsAlive())
	{
		if(bPlayerTurn)
		{
			m_player.Attack( *monster );
		}
		else
		{
			monster->Attack( m_player );
		}
		bPlayerTurn = !bPlayerTurn;
	}

	return m_player.IsAlive() ? true : false;
}

bool DungeonGameState::IsInBounds( FCoord& coord ) const noexcept
{
	return (coord.x >= 0 && coord.x < m_dungeonWidth &&
		coord.y >= 0 && coord.y < m_dungeonHeight );
}

void DungeonGameState::Initialize( int32 areaWidth , int32 areaHeight )
{
	
	m_player.GetCharacterInfo().position = { 1, 1 };
	m_currentDirection = EDirection::NONE;
	m_nextDirection = EDirection::NONE;
	m_bIsGameOver = false;

	ResetDungeon();

	m_player.Init();

}

void DungeonGameState::Reset()
{
	m_bIsGameOver = false;
	m_currentDirection = EDirection::NONE;

	ResetDungeon();
}

void DungeonGameState::Clear()
{
	m_dungeonMap.clear();
	m_statusMessage.clear();
	m_bIsGameOver = false;
	m_bPlayerEscaped = false;
	m_currentDirection = EDirection::NONE;
	m_nextDirection = EDirection::NONE;
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
	
	m_nextDirection = dir;
}
