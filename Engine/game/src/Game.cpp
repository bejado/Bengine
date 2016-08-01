#include "Game.h"
#include "ConsoleManager.h"
#include "GameUtils.h"

namespace BNG
{

	void Game::Update()
	{
		ConsoleManager::Get().HandleConsoleInput();
		ExitWithEscapeKey();
	}

}
