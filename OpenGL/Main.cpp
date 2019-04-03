#include "Game.h"


int main(void)
{
	bool Success = Game::Get().Initialize();
	if (Success)
	{
		Game::Get().Loop();
	}
	Game::Get().Shutdown();
















































	8
	return 0;
}
