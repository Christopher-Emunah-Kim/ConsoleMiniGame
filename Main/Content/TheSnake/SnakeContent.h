#pragma once
#include "../../IContent.h"
#include "../../../Core/Common.h"

class ScreenService;

constexpr int32 CELL_WIDTH = 2;
constexpr int32 BOARD_TOP_MARGIN = 6;

class SnakeContent final : public IContent
{
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

public:
	SnakeContent() = default;
	virtual ~SnakeContent() noexcept = default;

private:
	void BindInput();
	void ResetGame();
	void SpawnFood();
	
	void UpdateSnakePosition();
	void RenderGame( ScreenService& screen );
	void RenderUI( ScreenService& screen );

	void EnqueueDirection( EDirection dir );
	bool IsCollision( int32 x , int32 y ) const;

	void RequestExitToTitle();

public:
	virtual void OnInit() override;
	virtual void OnRelease() override;
	virtual void OnUpdate() override;
	virtual void OnRender() override;

private:
	vector<FCoord> m_snake;
	FCoord m_food;

	EDirection m_currentDirection = EDirection::NONE;
	EDirection m_nextDirection = EDirection::NONE;

	steady_clock::time_point m_lastMoveTime{};
	float m_moveInterval = 0.15f;

	int32 m_score = 0;

	bool m_bHasFood = false;
	bool m_bIsGameOver = false;
	bool m_bRequestExitToTitle = false;

	int32 m_gameAreaWidth = 25;
	int32 m_gameAreaHeight = 25;
};

