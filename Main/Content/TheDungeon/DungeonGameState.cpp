#include "DungeonGameState.h"

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

void DungeonGameState::Initialize( int32 areaWidth , int32 areaHeight )
{
}

void DungeonGameState::Reset()
{
	m_bIsGameOver = false;
	m_currentDirection = EDirection::NONE;
	m_player.Init();
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
