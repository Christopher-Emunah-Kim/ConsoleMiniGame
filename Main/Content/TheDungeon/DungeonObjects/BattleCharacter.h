#pragma once
#include "../../../../Core/Common.h"
#include "../DungeonContentType.h"


class BattleCharacter
{
public:
	BattleCharacter( const wstring& name );
	virtual ~BattleCharacter();

	virtual void Init();
	virtual void Attack( BattleCharacter& target );
	virtual void TakeDamage( int32 dammage );
	virtual bool IsAlive() const noexcept;

	inline FCharacterInfo& GetCharacterInfo() noexcept { return m_characterInfo; }

protected:
	FCharacterInfo m_characterInfo;
	int16 m_currentHP = DEFAULT_MAX_HEALTH;
};

