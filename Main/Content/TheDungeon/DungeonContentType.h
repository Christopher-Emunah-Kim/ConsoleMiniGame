#pragma once
#include "../../../Core/Common.h"
#include "../DefaultTypes.h"


constexpr int32 BOARD_TOP_MARGIN = 6;
constexpr int32 DUNGEON_AREA_WIDTH = 50;
constexpr int32 DUNGEON_AREA_HEIGHT = 50;

constexpr int16 DEFAULT_PLAYER_X = 1;
constexpr int16 DEFAULT_PLAYER_Y = 1;

constexpr int16 DEFAULT_MAX_HEALTH = 100;
constexpr int16 DEFAULT_ATTACK = 10;
constexpr int16 DEFAULT_DEFENSE = 5;
constexpr int16 DEFAULT_AGILITY = 5;

struct FCharacterInfo final
{
	FCoord position;
	wstring name;
	int8 attack;
	int8 defense;
	int8 agility;
	int16 maxHP;
};;