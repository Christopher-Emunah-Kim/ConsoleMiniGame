#pragma once
#include "../../IContent.h"
#include "SnakeGameState.h"
#include "SnakeRenderer.h"

class SnakeContent final : public IContent
{
public:
	SnakeContent() = default;
	virtual ~SnakeContent() noexcept = default;

private:
	void BindInput();
	void ResetGame();
	void RequestExitToTitle();

public:
	virtual void OnInit() override;
	virtual void OnRelease() override;
	virtual void OnUpdate() override;
	virtual void OnRender() override;

private:
	SnakeGameState m_gameState;
	SnakeRenderer m_renderer;
	bool m_bRequestExitToTitle = false;
};

