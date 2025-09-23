#include <iostream>
#include "Core/GameApplication.h"

int main()
{
	std::locale::global(std::locale("ko_KR.UTF-8"));
	std::srand(static_cast<unsigned int>(time(nullptr)));

	GameApplication app;

	if (false == app.Initialize())
	{
		return 0;
	}

	app.Run();

	return 0;
}
