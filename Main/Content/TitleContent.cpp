#include "TitleContent.h"
#include "../../Core/Common.h"
#include "../../Core/GameMaster.h"
#include "../../Service/InputService.h"

void TitleContent::LoadSnakeContent()
{
	GAME_MASTER->GetHUD().WriteLine( L"Snake 콘텐츠는 아직 준비 중입니다." );
}

void TitleContent::OnInit()
{
	auto& hud = GAME_MASTER->GetHUD();
	auto& input = GAME_MASTER->GetInputService();

	input.BindKeyAction(
		{
				{static_cast<InputService::InputKeyCode>( L'W' ), [this]()
						{
								GAME_MASTER->GetHUD().WriteLine( L"W 키 입력이 감지되었습니다." );
						}
				},
				{InputService::MakeKeyCode( InputService::KEY_PREFIX_EXTENDED, 72 ), [this]() // 72: 방향키 Up 스캔 코드
						{
								GAME_MASTER->GetHUD().WriteLine( L"위쪽 방향키 입력이 감지되었습니다." );
						}
				}
		}
	);

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
}


