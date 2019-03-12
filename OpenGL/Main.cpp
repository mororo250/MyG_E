#include "Game.h"


int main(void)
{
	Game game;
	bool Success = game.Initialize();
	if (Success)
	{
		game.Loop();
	}
	game.Shutdown();
	return 0;
}
