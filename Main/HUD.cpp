#include "HUD.h"
#include "../Core/GameMaster.h"

void HUD::Render(ScreenService& screen) const
{
	RenderInfoPanel(screen);
	RenderMainPanel(screen);
	RenderBottomPanel(screen);
}

void HUD::RenderInfoPanel(ScreenService& screen) const
{
	screen.Draw(0, 0, L"┌─────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────┐");

	for (int16 y = 1; y < SCREEN_HEIGHT - COMMAND_BLOCK_HEIGHT; ++y)
	{
		screen.Draw(0, y, L"│");
		screen.Draw(PLAYERINFO_PANEL_WIDTH, y, L"│");
		screen.Draw(SCREEN_WIDTH - 2, y, L"│");
	}

	screen.Draw(LEFT_MARGIN + 5, 1, L"[ WELCOM TO TRPG ]");
	screen.Draw((int32)(SCREEN_WIDTH * 0.55f), 1, L"<< ");
	screen.Draw(LEFT_MARGIN - 2, PLAYER_UI_BASE_Y + 1, L"───────────────────────────────");
	screen.Draw(GAME_PANEL_START_X, PLAYER_UI_BASE_Y + 1, L"────────────────────────────────────────────────────────────────────────────────────────────");
	screen.Draw(LEFT_MARGIN, PLAYER_UI_BASE_Y + 3, L"     [ 플레이어  정보 ]");
	screen.Draw(LEFT_MARGIN - 2, PLAYER_UI_BASE_Y + 4, L"───────────────────────────────");
	PLAYER_UI_BASE_Y + 5;
	screen.Draw(LEFT_MARGIN, PLAYER_UI_BASE_Y + 6, L"아이디 : " );
	PLAYER_UI_BASE_Y + 7;
	screen.Draw(LEFT_MARGIN, PLAYER_UI_BASE_Y + 8, L"레벨 : " );
	PLAYER_UI_BASE_Y + 9;
	screen.Draw(LEFT_MARGIN - 2, PLAYER_UI_BASE_Y + 10, L"───────────────────────────────");
	PLAYER_UI_BASE_Y + 11;
	screen.Draw(LEFT_MARGIN, PLAYER_UI_BASE_Y + 12, L"체력 : " );
	PLAYER_UI_BASE_Y + 13;
	screen.Draw(LEFT_MARGIN, PLAYER_UI_BASE_Y + 14, L"경험치 : " );
	PLAYER_UI_BASE_Y + 15;
	screen.Draw(LEFT_MARGIN, PLAYER_UI_BASE_Y + 16, L"보유 금액 : " );
	PLAYER_UI_BASE_Y + 17;
	screen.Draw(LEFT_MARGIN - 2, PLAYER_UI_BASE_Y + 18, L"───────────────────────────────");
	screen.Draw(LEFT_MARGIN, PLAYER_UI_BASE_Y + 19, L"  [ 플레이어  스테이터스 ]");
	screen.Draw(LEFT_MARGIN - 2, PLAYER_UI_BASE_Y + 20, L"───────────────────────────────");
	PLAYER_UI_BASE_Y + 21;
	screen.Draw(LEFT_MARGIN, PLAYER_UI_BASE_Y + 22, L"공격력 : " );
	PLAYER_UI_BASE_Y + 23;
	screen.Draw(LEFT_MARGIN, PLAYER_UI_BASE_Y + 24, L"방어력 : " );
	PLAYER_UI_BASE_Y + 25;
	screen.Draw(LEFT_MARGIN, PLAYER_UI_BASE_Y + 26, L"민첩성 : " );
	PLAYER_UI_BASE_Y + 27;
	screen.Draw(LEFT_MARGIN - 2, PLAYER_UI_BASE_Y + 28, L"───────────────────────────────");
	screen.Draw(LEFT_MARGIN, PLAYER_UI_BASE_Y + 29, L"사용 무기: " );
	PLAYER_UI_BASE_Y + 30;
	screen.Draw(LEFT_MARGIN, PLAYER_UI_BASE_Y + 31, L"사용 방어구: " );
	PLAYER_UI_BASE_Y + 32;
	screen.Draw(LEFT_MARGIN, PLAYER_UI_BASE_Y + 33, L"가방: ");

	/*if (m_hudData.inventoryItems.empty())
	{
		screen.Draw(LEFT_MARGIN + 6, PLAYER_UI_BASE_Y + 33, L"없음");
	}
	else
	{
		screen.Draw(LEFT_MARGIN + 6, PLAYER_UI_BASE_Y + 33, L"[" + m_hudData.inventoryItems[0] + L"]");

		for (size_t i = 1; i < m_hudData.inventoryItems.size(); ++i)
		{
			screen.Draw(LEFT_MARGIN + 6, PLAYER_UI_BASE_Y + 33 + (int32)i,
				L"[" + m_hudData.inventoryItems[i] + L"]");
		}
	}*/

	screen.Draw(0, SCREEN_HEIGHT - COMMAND_BLOCK_HEIGHT, L"│─────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────│");
	screen.Draw(0, SCREEN_HEIGHT - COMMAND_BLOCK_HEIGHT + 1, L"│");



	screen.Draw(SCREEN_WIDTH - 2, SCREEN_HEIGHT - COMMAND_BLOCK_HEIGHT + 1, L"│");
	screen.Draw(0, SCREEN_HEIGHT - 1, L"└─────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────┘");

}

void HUD::RenderMainPanel(ScreenService& screen) const
{
	int32 messageAreaWidth = SCREEN_WIDTH - GAME_PANEL_START_X - RIGHT_MARGIN;
	wstring clearLine(messageAreaWidth, L' ');

	for (int32 y = SYSTEM_TEXT_BASE_Y + 1; y < SYSTEM_TEXT_BASE_Y + MAX_LINES; ++y)
	{
		screen.Draw(GAME_PANEL_START_X + 1, y, clearLine);
	}

	int32 outputY = SYSTEM_TEXT_BASE_Y + 1;
	for (size_t i = 0; i < m_mainTextQueue.size(); ++i)
	{
		screen.Draw(GAME_PANEL_START_X + 1, outputY, m_mainTextQueue[i]);
		++outputY;
	}
}

void HUD::RenderBottomPanel(ScreenService& screen) const
{
	screen.Draw(2, SCREEN_HEIGHT - COMMAND_BLOCK_HEIGHT + 1, L"명령 > ");
	screen.Draw(9, SCREEN_HEIGHT - COMMAND_BLOCK_HEIGHT + 1, GAME_MASTER->GetInputService().GetInputBuffer());
}

void HUD::WriteLine(const wstring& line)
{
	if (m_mainTextQueue.size() >= MAX_LINES)
	{
		m_mainTextQueue.pop_front();
	}

	m_mainTextQueue.push_back(line);
}

void HUD::ClearText()
{
	m_mainTextQueue.clear();
}
