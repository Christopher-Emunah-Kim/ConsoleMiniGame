#include "TitleContent.h"
#include "../../Core/Common.h"
#include "../../Core/GameMaster.h"

void TitleContent::LoadSnakeContent()
{
}

void TitleContent::OnInit()
{
}

void TitleContent::OnRelease()
{
}

void TitleContent::OnUpdate()
{
	GAME_MASTER->GetInputService().BindAction(
		{
			{L"1", bind(&TitleContent::LoadSnakeContent, this)}
		}
	);

	GAME_MASTER->GetInputService().BindActionOnInputError([this]()
		{
			GAME_MASTER->GetHUD().WriteLine( L"Invalid Command in TitleContent" );
		} 
	);
	
}

void TitleContent::OnRender()
{
	GAME_MASTER->GetHUD().WriteLine( L"■■■■■  ■          ■      ■  ■■■■■  " );
	GAME_MASTER->GetHUD().WriteLine( L"■      ■  ■          ■      ■  ■          " );
	GAME_MASTER->GetHUD().WriteLine( L"■      ■  ■          ■      ■  ■          " );
	GAME_MASTER->GetHUD().WriteLine( L"■■■■■  ■          ■      ■  ■■■■■  " );
	GAME_MASTER->GetHUD().WriteLine( L"■          ■          ■      ■          ■  " );
	GAME_MASTER->GetHUD().WriteLine( L"■          ■          ■      ■          ■  " );
	GAME_MASTER->GetHUD().WriteLine( L"■          ■■■■■  ■■■■■  ■■■■■  " );

	
	GAME_MASTER->GetHUD().WriteLine( L"Console Game Pack ver 0.3" );
	GAME_MASTER->GetHUD().WriteLine( L" " );
	GAME_MASTER->GetHUD().WriteLine( L" " );
	GAME_MASTER->GetHUD().WriteLine( L"타이틀 화면입니다." );
	GAME_MASTER->GetHUD().WriteLine( L"시작하시려면 1번, 종료하시려면 2번을 눌러주세요." );
}


