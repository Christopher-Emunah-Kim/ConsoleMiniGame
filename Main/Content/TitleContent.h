#pragma once
#include "../../Core/Common.h"
#include "../IContent.h"

class TitleContent final : public IContent
{
public:
	TitleContent() = default;
	virtual ~TitleContent() noexcept = default;

private:
	struct FMenuItem
	{
		wstring label;
		function<void()> action;
	};

	void LoadSnakeContent();

	void ExitGame();

	void InitializeMenu();
	void MoveToPreviousMenu();
	void MoveToNextMenu();
	void SelectMenuItem();

	void BindInputActions();

	void RenderTitleScreen();

public:
	virtual void OnInit() override;
	virtual void OnRelease() override;
	virtual void OnUpdate() override;
	virtual void OnRender() override;

private:
	vector<FMenuItem> m_menuItems;
	size_t m_selectedIndex = 0;
};

