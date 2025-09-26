#pragma once
#include "SnakeType.h"
#include <vector>

class SnakeGameState final
{
private:
	void UpdateSnakePosition();
	void SpawnFood();
	bool IsCollision( int32 x , int32 y ) const;

public:
	void Initialize( int32 areaWidth , int32 areaHeight, float moveIntervalSeconds );
	void Reset();
	void Clear();

	void EnqueueDirection( EDirection dir );

	bool TryMove();

	inline bool IsGameOver() const noexcept { return m_bIsGameOver; }
	inline bool HasFood() const noexcept { return m_bHasFood; }
	inline int32 GetScore() const noexcept { return m_score; }
	inline int32 GetWidth() const noexcept { return m_gameAreaWidth; }
	inline int32 GetHeight() const noexcept { return m_gameAreaHeight; }
	inline const vector<FCoord>& GetSnake() const { return m_snake; }
	inline const FCoord& GetFood() const { return m_food; }

private:
	vector<FCoord> m_snake;
	FCoord m_food;

	EDirection m_currentDirection = EDirection::NONE;
	EDirection m_nextDirection = EDirection::NONE;

	steady_clock::time_point m_lastMoveTime{};

	float m_moveInterval = 0.15f;

	int32 m_score = 0;

	bool m_bHasFood = false;
	bool m_bIsGameOver = false;

	int32 m_gameAreaWidth = SNAKEGAME_AREA_WIDTH;
	int32 m_gameAreaHeight = SNAKEGAME_AREA_HEIGHT;
};

