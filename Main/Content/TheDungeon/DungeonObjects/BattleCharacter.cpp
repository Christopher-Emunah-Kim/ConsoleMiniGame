#include "BattleCharacter.h"

BattleCharacter::BattleCharacter( const wstring& name )
	: m_currentHP( DEFAULT_MAX_HEALTH ) , m_characterInfo{ {DEFAULT_PLAYER_X, DEFAULT_PLAYER_Y}, name , 
	DEFAULT_ATTACK , DEFAULT_DEFENSE , DEFAULT_AGILITY , DEFAULT_MAX_HEALTH }
{
}

BattleCharacter::~BattleCharacter()
{
}

void BattleCharacter::Init()
{
	m_currentHP = m_characterInfo.maxHP;
}

void BattleCharacter::Attack( BattleCharacter& target )
{
	FCharacterInfo& attackerInfo = m_characterInfo;
	FCharacterInfo& targetInfo = target.m_characterInfo;

	int32 damage = attackerInfo.attack - targetInfo.defense;
	damage = max( 1 , damage );

	target.TakeDamage( damage );
}

void BattleCharacter::TakeDamage( int32 dammage )
{
	m_currentHP = static_cast<int16>( max( 0 , m_currentHP - dammage ) );
}

bool BattleCharacter::IsAlive() const noexcept
{
	return m_currentHP > 0;
}
