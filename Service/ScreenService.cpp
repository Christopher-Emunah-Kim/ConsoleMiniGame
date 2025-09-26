#include "ScreenService.h"

ScreenService::~ScreenService()
{
	Release();
}

void ScreenService::VisibleConsoleCursor(bool isVisible)
{
	CONSOLE_CURSOR_INFO cursorInfo;

	GetConsoleCursorInfo(m_consoleBuffers[0], &cursorInfo);
	cursorInfo.bVisible = isVisible;
	SetConsoleCursorInfo(m_consoleBuffers[0], &cursorInfo);

	GetConsoleCursorInfo(m_consoleBuffers[1], &cursorInfo);
	cursorInfo.bVisible = isVisible;
	SetConsoleCursorInfo(m_consoleBuffers[1], &cursorInfo);
}

bool ScreenService::IsValidCoordinate(int32 x, int32 y)
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

bool ScreenService::IsDoubleWidthCharacter(const wchar_t& c) const
{
	//기존 한글처리
	// 0xAC00(가) ~ 0xD7A3(핳)
	/*if (c >= (int32)0xAC00 && c <= (int32)0xD7A3)
	{
		return true;
	}

	return false;*/

	// Windows 콘솔에서 두 칸 폭으로 표현되는 주요 유니코드 영역을 확인
	if ( c < 0x1100 )
	{
		return false;
	}

	struct Interval
	{
		wchar_t first;
		wchar_t last;
	};

	static const Interval wideIntervals[] = {
		{ 0x1100, 0x115F }, // 한글 자모
		{ 0x2329, 0x232A },
		{ 0x2E80, 0x2FFB }, // CJK 부수, 한자 호환 등
		{ 0x3000, 0x303E },
		{ 0x3040, 0x4DBF },
		{ 0x4E00, 0xA4C6 },
		{ 0xA960, 0xA97C },
		{ 0xAC00, 0xD7FB }, // 한글 음절
		{ 0xF900, 0xFAFF },
		{ 0xFE10, 0xFE19 },
		{ 0xFE30, 0xFE6B },
		{ 0xFF01, 0xFF60 },
		{ 0xFFE0, 0xFFE6 },
		{ 0x1F200, 0x1F6FF }, // 기호, 도형, 이모지 등
		{ 0x20000, 0x3FFFD }
	};

	for ( const auto& interval : wideIntervals )
	{
		if ( c >= interval.first && c <= interval.last )
		{
			return true;
		}
	}

	// 동그라미, 별, 네모 등은 모호 폭(ambiguous width)이라는데
	// 스네이크 게임에서 2칸 폭으로 표시되므로 예외적으로 true처리
	if ( c >= 0x25A0 && c <= 0x26FF )
	{
		return true;
	}

	return false;
}

void ScreenService::SetFixedWindowSize()
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

void ScreenService::SetFontColor(int8 color, int8 bgColor)
{
	if (bgColor < BLACK || bgColor > WHITE)
	{
		bgColor = BLACK;
	}

	m_currentAttribute = static_cast<WORD>( ( ( bgColor & 0xf ) << 4 ) | ( color & 0xf ) );
	SetConsoleTextAttribute(m_consoleBuffers[m_backBufferIdx], m_currentAttribute );
}

void ScreenService::Init()
{
	CreateBuffer();

	Clear();

	SetFixedWindowSize();

	VisibleConsoleCursor(false);
}

void ScreenService::CreateBuffer()
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

void ScreenService::Release()
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
	m_currentAttribute = static_cast<WORD>( ( ( DEFAULT_BG_COLOR & 0xf ) << 4 ) | ( DEFAULT_FONT_COLOR & 0xf ) );
}

void ScreenService::Clear()
{
	for (int32 y = 0; y < SCREEN_HEIGHT; ++y)
	{
		for (int32 x = 0; x < SCREEN_WIDTH; ++x)
		{
			m_writeBuffer[y][x] = L' ';
			m_colorBuffer[ y ][ x ] = static_cast<WORD>( ( ( DEFAULT_BG_COLOR & 0xf ) << 4 ) | ( DEFAULT_FONT_COLOR & 0xf ) );
		}
	}
	m_currentAttribute = static_cast<WORD>( ( ( DEFAULT_BG_COLOR & 0xf ) << 4 ) | ( DEFAULT_FONT_COLOR & 0xf ) );
}

void ScreenService::SwapBuffer()
{
	HANDLE hBackBuffer = m_consoleBuffers[m_backBufferIdx];

	vector<CHAR_INFO> tempBuffer(SCREEN_HEIGHT * SCREEN_WIDTH);

	for (int32 y = 0; y < SCREEN_HEIGHT; ++y)
	{
		for (int32 x = 0; x < SCREEN_WIDTH; ++x)
		{
			tempBuffer[y * SCREEN_WIDTH + x].Char.UnicodeChar = m_writeBuffer[y][x];
			tempBuffer[y * SCREEN_WIDTH + x].Attributes = m_colorBuffer[y][x];
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

void ScreenService::Draw(int32 x, int32 y, const wchar_t& c)
{
	if (false == IsValidCoordinate(x, y))
	{
		return;
	}

	m_writeBuffer[y][x] = c;
	m_colorBuffer[ y ][ x ] = m_currentAttribute;

	if ( IsDoubleWidthCharacter( c ) )
	{
		int32 nextX = x + 1;
		if(IsValidCoordinate( nextX , y ) )
		{
			m_writeBuffer[y][nextX] = L' ';
			m_colorBuffer[ y ][ nextX ] = m_currentAttribute;
		}
	}
}

void ScreenService::Draw(int32 x, int32 y, const wstring& str, int8 color, int8 bgColor)
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

		if (IsDoubleWidthCharacter(str[i]))
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
