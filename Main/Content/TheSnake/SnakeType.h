#pragma once
#include "../../../Core/Common.h"


constexpr int32 CELL_WIDTH = 2;
constexpr int32 BOARD_TOP_MARGIN = 6;

enum class EDirection : int8
{
	NONE = 0 ,
	UP ,
	DOWN ,
	LEFT ,
	RIGHT
};

struct FCoord final
{
	int32 x = 0;
	int32 y = 0;
};