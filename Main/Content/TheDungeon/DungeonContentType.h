#pragma once
#include "../../../Core/Common.h"
#include "../DefaultTypes.h"


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
};

struct FDungeonGeneratingParams final
{
	int32 width = DUNGEON_AREA_WIDTH;
	int32 height = DUNGEON_AREA_HEIGHT;
	double wallDensity = 0.38;
	double monsterProb = 0.07;
	double	itemProb = 0.04;
};

enum Cell : int8
{
	PATH = 0 ,
	WALL = 1 ,
	START = 2 ,
	MONSTER = 3,
	ITEM = 4,
	EXIT = 5
};

