#pragma once
#include "BattleCharacter.h"

class DungeonMonster final : public BattleCharacter
{
public:
	DungeonMonster( const wstring& name );
	virtual ~DungeonMonster() noexcept = default;
};

