#include "Screen.h"

Screen::~Screen()
{
	Release();
}

void Screen::VisibleConsoleCursor(bool isVisible)
{
	CONSOLE_CURSOR_INFO cursorInfo;

	GetConsoleCursorInfo(m_consoleBuffers[0], &cursorInfo);
	cursorInfo.bVisible = isVisible;
	SetConsoleCursorInfo(m_consoleBuffers[0], &cursorInfo);

	GetConsoleCursorInfo(m_consoleBuffers[1], &cursorInfo);
	cursorInfo.bVisible = isVisible;
	SetConsoleCursorInfo(m_consoleBuffers[1], &cursorInfo);
}

bool Screen::IsValidCoordinate(int32 x, int32 y)
{
	if (0 <= x && x < SCREEN_WIDTH && 0 <= y && y < SCREEN_HEIGHT)
	{
		return true;
	}
	else
	{
		return false;
	}
}

bool Screen::IsHangulSyllable(const wchar_t& c) const
{
	// 0xAC00(°¡) ~ 0xD7A3(ÁK)
	if (c >= (int32)0xAC00 && c <= (int32)0xD7A3)
	{
		return true;
	}

	return false;
}

void Screen::SetFixedWindowSize()
{
	HWND consoleWindow = GetConsoleWindow();
	if (consoleWindow == nullptr)
		return;

	CONSOLE_FONT_INFO fontInfo;
	GetCurrentConsoleFont(GetStdHandle(STD_OUTPUT_HANDLE), FALSE, &fontInfo);

	COORD fontSize = GetConsoleFontSize(GetStdHandle(STD_OUTPUT_HANDLE), fontInfo.nFont);

	int windowWidth = fontSize.X * SCREEN_WIDTH;
	int windowHeight = fontSize.Y * SCREEN_HEIGHT;

	MoveWindow(consoleWindow, 100, 100, windowWidth, windowHeight, TRUE);
}

void Screen::SetFontColor(int8 color, int8 bgColor)
{
	if (bgColor < BLACK || bgColor > WHITE)
	{
		bgColor = BLACK;
	}

	SetConsoleTextAttribute(m_consoleBuffers[m_backBufferIdx], ((bgColor & 0xf) << 4) | (color & 0xf));
}

void Screen::Init()
{
	CreateBuffer();

	Clear();

	SetFixedWindowSize();

	VisibleConsoleCursor(false);
}

void Screen::CreateBuffer()
{
	//Double Buffering
	//1. Create two screen buffers
	for (int32 i = 0; i < BUFFER_SIZE; ++i)
	{
		m_consoleBuffers[i] = CreateConsoleScreenBuffer(
			GENERIC_READ | GENERIC_WRITE, //dwDesiredAccess
			0, nullptr, // dwShareMode,*lpSecurityAttributes
			CONSOLE_TEXTMODE_BUFFER, //dwFlags
			nullptr //lpScreenBufferData
		);

		if (m_consoleBuffers[i] == INVALID_HANDLE_VALUE)
		{
			continue;
		}

		//2. Set each screen buffer size
		SetConsoleScreenBufferSize(m_consoleBuffers[i], { SCREEN_WIDTH, SCREEN_HEIGHT });

		SMALL_RECT rect = {
			0, //left
			0, //top
			SCREEN_WIDTH - 1, //right 
			SCREEN_HEIGHT - 1 //bottom
		};

		//3. Set each screen buffer window size
		if (!SetConsoleWindowInfo(m_consoleBuffers[i], TRUE, &rect))
		{
			CloseHandle(m_consoleBuffers[i]);
			m_consoleBuffers[i] = INVALID_HANDLE_VALUE;
			continue;
		}
	}

	//4. Set the first buffer as the active screen buffer
	m_backBufferIdx = 0;
}

void Screen::Release()
{
	VisibleConsoleCursor(true);
	for (int32 i = 0; i < BUFFER_SIZE; ++i)
	{
		if (m_consoleBuffers[i] != INVALID_HANDLE_VALUE)
		{
			CloseHandle(m_consoleBuffers[i]);
			m_consoleBuffers[i] = INVALID_HANDLE_VALUE;
		}
	}
}

void Screen::Clear()
{
	for (int32 y = 0; y < SCREEN_HEIGHT; ++y)
	{
		for (int32 x = 0; x < SCREEN_WIDTH; ++x)
		{
			m_writeBuffer[y][x] = L' ';
		}
	}
}

void Screen::SwapBuffer()
{
	HANDLE hBackBuffer = m_consoleBuffers[m_backBufferIdx];

	vector<CHAR_INFO> tempBuffer(SCREEN_HEIGHT * SCREEN_WIDTH);

	for (int32 y = 0; y < SCREEN_HEIGHT; ++y)
	{
		for (int32 x = 0; x < SCREEN_WIDTH; ++x)
		{
			tempBuffer[y * SCREEN_WIDTH + x].Char.UnicodeChar = m_writeBuffer[y][x];
			tempBuffer[y * SCREEN_WIDTH + x].Attributes = FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE;
		}
	}

	COORD bufferSize = { SCREEN_WIDTH, SCREEN_HEIGHT };
	COORD bufferCoord = { 0, 0 };

	SMALL_RECT writeRegion = {
		0,
		0,
		SCREEN_WIDTH - 1,
		SCREEN_HEIGHT - 1 };

	WriteConsoleOutputW(hBackBuffer, tempBuffer.data(), bufferSize, bufferCoord, &writeRegion);

	SetConsoleActiveScreenBuffer(hBackBuffer);

	m_backBufferIdx = (m_backBufferIdx + 1) % 2;
}

void Screen::Draw(int32 x, int32 y, const wchar_t& c)
{
	if (false == IsValidCoordinate(x, y))
	{
		return;
	}

	m_writeBuffer[y][x] = c;
}

void Screen::Draw(int32 x, int32 y, const wstring& str, int8 color, int8 bgColor)
{
	SetFontColor(color, bgColor);
	
	if (false == IsValidCoordinate(x, y))
	{
		return;
	}

	int32 currentX = x;

	for (size_t i = 0; i < str.length(); ++i)
	{
		Draw(currentX, y, str[i]);

		if (IsHangulSyllable(str[i]))
		{
			currentX += 2;
		}
		else
		{
			currentX += 1;
		}
	}

	SetFontColor(DEFAULT_FONT_COLOR, DEFAULT_BG_COLOR);
}
