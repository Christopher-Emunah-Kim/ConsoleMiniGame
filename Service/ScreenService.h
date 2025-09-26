#pragma once
#include "../Core/Common.h"

constexpr int32 SCREEN_WIDTH = 128;
constexpr int32 SCREEN_HEIGHT = 42;
constexpr int8 BUFFER_SIZE = 2;
constexpr int8 DEFAULT_FONT_COLOR = WHITE;
constexpr int8 DEFAULT_BG_COLOR = BLACK;

class ScreenService final
{
public:
	virtual ~ScreenService();

private:
	void VisibleConsoleCursor(bool isVisible);
	bool IsValidCoordinate(int32 x, int32 y);
	bool IsDoubleWidthCharacter(const wchar_t& c) const;
	void SetFixedWindowSize();
	void SetFontColor(int8 color, int8 bgColor);

public:
	void Init();
	void CreateBuffer();
	void Release();
	void Clear();
	void SwapBuffer();
	void Draw(int32 x, int32 y, const wchar_t& c);
	void Draw(int32 x, int32 y, const wstring& str, int8 color = WHITE, int8 bgColor = BLACK);

private:
	HANDLE m_consoleBuffers[BUFFER_SIZE] = { INVALID_HANDLE_VALUE, INVALID_HANDLE_VALUE };
	int32 m_backBufferIdx = 0;
	WCHAR m_writeBuffer[SCREEN_HEIGHT][SCREEN_WIDTH] = { };

	//for colors
	WORD m_colorBuffer[ SCREEN_HEIGHT ][ SCREEN_WIDTH ] = { };
	WORD m_currentAttribute = static_cast<WORD>(( ( DEFAULT_BG_COLOR & 0xf ) << 4 ) | ( DEFAULT_FONT_COLOR & 0xf ));
};