#pragma once

#include <Windows.h>
#include <conio.h>
#include <iomanip>

#include <functional>
#include <format>
#include <expected>

#include <vector>
#include <string>
#include <set>
#include <unordered_map>
#include <array>
#include <list>
#include <map>
#include <algorithm>

#include <chrono>
#include <ctime>

#include <iostream>
#include <fstream>
#include <sstream>


#define GAME_MASTER GameMaster::GetInstance()

typedef signed __int8       int8;     //-128~127
typedef signed __int16      int16;   //-32768~32767
typedef signed __int32      int32;   //-2147483648~2147483647
typedef signed __int64      int64;   //-9223372036854775808~9223372036854775807
typedef unsigned __int8     uint8;   //0~255
typedef unsigned __int16    uint16;  //0~65535
typedef unsigned __int32    uint32;  //0~4294967295
typedef unsigned __int64    uint64;  //0~18446744073709551615

using namespace std;
using namespace std::chrono;

enum class EAreaName {
	TOWN,
	FOREST,
	CAVE,
	CASTLE
};

enum class EContent {
	INTRO,
	TITLE,
	LOAD
};

// Color code for Windows Console
#define BLACK		0
#define DARKBLUE	1
#define DARKGREEN	2
#define DARKSKYBLUE 3
#define DARKRED		4
#define DARKPURPLE	5
#define DARKYELLOW	6
#define GRAY		7
#define DARKGRAY	8
#define BLUE		9
#define GREEN		10
#define SKYBLUE		11
#define RED			12
#define PURPLE		13
#define YELLOW 		14
#define WHITE		15
