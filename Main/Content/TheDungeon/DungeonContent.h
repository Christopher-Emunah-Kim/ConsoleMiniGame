#pragma once
#include "../../IContent.h"
#include "DungeonGameState.h"
#include "DungeonRenderer.h"

class DungeonContent final : public IContent
{
public:
	DungeonContent() = default;
	virtual ~DungeonContent() noexcept = default;

private:
	void BindInput();
	void ResetGame();
	void ReqeustExitToTitle();

public:
	virtual void OnInit() override;
	virtual void OnRelease() override;
	virtual void OnUpdate() override;
	virtual void OnRender() override;

private:
	DungeonGameState m_gameState;
	DungeonRenderer m_renderer;
	bool m_bRequestExitToTitle = false;
};

