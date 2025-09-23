#pragma once
#include "../Service/ScreenService.h"

class GameApplication
{
public:
	virtual ~GameApplication() noexcept;

private:
	void ProcessInput();
	void Update();
	void Render();
	void Release();

public:
	bool Initialize();
	void Run();

private:
	ScreenService m_screen;
};

