#include "Game.h"
#include "ConsoleManager.h"
#include "GameUtils.h"

namespace ITP485
{

	void Game::Update()
	{
		ConsoleManager::Get().HandleConsoleInput();
		ExitWithEscapeKey();
	}

}
