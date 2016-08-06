#include "Game.h"
#include "ConsoleManager.h"
#include "GameUtils.h"
#include "Renderer.h"

namespace BNG
{

	void Game::Update()
	{
		ConsoleManager::Get().HandleConsoleInput();
		ExitWithEscapeKey();
	}

	void Game::Render()
	{
		Renderer::Get().Render();
	}

	void Game::HandleMessage( const json& msg )
	{
		// do nothing, subclasses can override
	}

}
