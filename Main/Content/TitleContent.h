#pragma once
#include "../IContent.h"

class TitleContent final : public IContent
{
public:
	TitleContent() = default;
	virtual ~TitleContent() noexcept = default;

private:
	void LoadSnakeContent();
	void ExitGame();

public:
	virtual void OnInit() override;
	virtual void OnRelease() override;
	virtual void OnUpdate() override;
	virtual void OnRender() override;
};

