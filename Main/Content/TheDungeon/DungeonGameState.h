#pragma once
#include "../DefaultTypes.h"
#include "DungeonContentType.h"
#include "DungeonObjects/DungeonPlayer.h"
#include "DungeonObjects/DungeonMonster.h"

class DungeonGameState
{
private:
	bool UpdatePlayerPosition();
	vector<vector<int>> GenerateDungeonMap( const FDungeonGeneratingParams& params );
	void ResetDungeon();
	void CarveLPath( vector<vector<int>>& grid );

	void HandleCurrentCellEvent(const FCoord& nextPos, int8 cellType);
	void HandleMonsterEvent( const FCoord& nextPos );
	bool ProcessBattle( DungeonMonster* monster );
	bool IsInBounds( FCoord& coord ) const noexcept;

public:
	void Initialize( int32 areaWidth , int32 areaHeight );
	void Reset();
	void Clear();

	void EnqueueDirection( EDirection dir );
	bool TryPlayerMove( );

	inline bool IsGameOver() const noexcept { return m_bIsGameOver; }
	inline bool HasPlayerEscaped() const noexcept { return m_bPlayerEscaped; }
	inline const DungeonPlayer& GetPlayer() const noexcept { return m_player; }
	inline const vector<vector<int>>& GetDungeonMap() const noexcept { return m_dungeonMap; }
	inline const wstring& GetStatusMessage() const noexcept { return m_statusMessage; }

private:
	EDirection m_currentDirection = EDirection::NONE;
	EDirection m_nextDirection = EDirection::NONE;
	DungeonPlayer m_player{ L"Hero" };

	vector<vector<int>> m_dungeonMap;
	int32 m_dungeonWidth = DUNGEON_AREA_WIDTH;
	int32 m_dungeonHeight = DUNGEON_AREA_HEIGHT;

	bool m_bIsGameOver = false;
	bool m_bPlayerEscaped = false;

	wstring m_statusMessage;
};

