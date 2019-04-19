#include "Game.h"


int main(void)
{
	bool Success = Game::Get().Initialize();
	if (Success)
	{
		Game::Get().Loop();
	}
	Game::Get().Shutdown();
	return 0;
}
