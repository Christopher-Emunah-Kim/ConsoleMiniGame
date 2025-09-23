#pragma once
#include "Screen.h"

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
	Screen m_screen;
};

