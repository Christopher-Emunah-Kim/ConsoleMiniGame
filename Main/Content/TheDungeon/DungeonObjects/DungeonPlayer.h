#pragma once
#include "BattleCharacter.h"

class DungeonPlayer final : public BattleCharacter
{

public:
	DungeonPlayer( const wstring& name );
	virtual ~DungeonPlayer() noexcept = default;

	void Heal( int32 amount );

private:
	int32 m_gold = 0;
	int32 m_experience = 0;
	int16 m_level = 1;
};

