#pragma once
#include "../DefaultTypes.h"
#include "DungeonContentType.h"
#include "DungeonObjects/DungeonPlayer.h"

class DungeonGameState
{
private:
	void UpdatePlayerPosition();
	vector<vector<int>> GenerateDungeonMap( const FDungeonGeneratingParams& params );
	void CarveLPath( vector<vector<int>>& grid );

public:
	void Initialize( int32 areaWidth , int32 areaHeight );
	void Reset();
	void Clear();

	void EnqueueDirection( EDirection dir );
	bool TryPlayerMove( );

	inline bool IsGameOver() const noexcept { return m_bIsGameOver; }

private:
	EDirection m_currentDirection = EDirection::NONE;
	EDirection m_nextDirection = EDirection::NONE;
	DungeonPlayer m_player{ L"Hero" };

	bool m_bIsGameOver = false;
};

