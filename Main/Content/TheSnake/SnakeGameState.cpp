#include "SnakeGameState.h"
#include <algorithm>

void SnakeGameState::UpdateSnakePosition() 
{
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

	if ( head.x < 0 || head.x >= m_gameAreaWidth || head.y < 0 || head.y >= m_gameAreaHeight )
	{
		m_bIsGameOver = true;
		return;
	}

	if ( IsCollision( head.x , head.y ) )
	{
		m_bIsGameOver = true;
		return;
	}

	m_snake.insert( m_snake.begin() , head );

	if ( m_bHasFood && head.x == m_food.x && head.y == m_food.y )
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

void SnakeGameState::SpawnFood() 
{
	if ( m_snake.empty() )
	{
		return;
	}

	const int32 maxCells = m_gameAreaWidth * m_gameAreaHeight;

	if ( static_cast<int32>( m_snake.size() ) >= maxCells )
	{
		// No space to spawn food
		m_bHasFood = false;
		return;
	}

	for ( int32 tryCount = 0; tryCount < maxCells; ++tryCount )
	{
		const int32 x = rand() % m_gameAreaWidth;
		const int32 y = rand() % m_gameAreaHeight;

		if ( !IsCollision(x, y) )
		{
			m_food = { x , y };
			m_bHasFood = true;
			return;
		}
	}

	m_bHasFood = false;
}

bool SnakeGameState::IsCollision( int32 x , int32 y ) const
{
	for ( const FCoord& segment : m_snake )
	{
		if ( segment.x == x && segment.y == y )
		{
			return true;
		}
	}

	return false;
}

void SnakeGameState::Initialize( int32 areaWidth , int32 areaHeight, float moveIntervalSeconds ) 
{
	m_gameAreaWidth = max<int32>(1, areaWidth );
	m_gameAreaHeight = max<int32>(1, areaHeight);
	m_moveInterval = moveIntervalSeconds;
}

void SnakeGameState::Reset() 
{
	m_snake.clear();
	m_snake.push_back({ m_gameAreaWidth /2 , m_gameAreaHeight / 2});
	m_snake.push_back({ m_gameAreaWidth /2 - 1, m_gameAreaHeight / 2});
	m_snake.push_back( { m_gameAreaWidth / 2 - 2, m_gameAreaHeight / 2 } );

	m_currentDirection = EDirection::RIGHT;
	m_nextDirection = EDirection::RIGHT;

	m_score = 0;
	m_bHasFood = false;
	m_bIsGameOver = false;

	m_lastMoveTime = steady_clock::now();

	SpawnFood();
}

void SnakeGameState::Clear() 
{
	m_snake.clear();
	m_bHasFood = false;
	m_bIsGameOver = false;
	m_score = 0;
	m_currentDirection = EDirection::NONE;
	m_nextDirection = EDirection::NONE;
}

void SnakeGameState::EnqueueDirection( EDirection dir ) 
{
	if ( dir == EDirection::NONE || m_bIsGameOver )
	{
		return;
	}

	if ( ( dir == EDirection::UP && m_currentDirection == EDirection::DOWN ) ||
		( dir == EDirection::DOWN && m_currentDirection == EDirection::UP ) ||
		( dir == EDirection::LEFT && m_currentDirection == EDirection::RIGHT ) ||
		( dir == EDirection::RIGHT && m_currentDirection == EDirection::LEFT ) )
	{
		return;
	}

	m_nextDirection = dir;
}

bool SnakeGameState::TryMove()
{
	if ( m_bIsGameOver || m_snake.empty() )
	{
		return false;
	}

	const steady_clock::time_point now = steady_clock::now();
	const duration<float> deltaTime = now - m_lastMoveTime;

	if ( deltaTime.count() < m_moveInterval )
	{
		return false;
	}

	m_lastMoveTime = now;

	UpdateSnakePosition();
	return true;
}
