#pragma once
#include "../../Core/Common.h"

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