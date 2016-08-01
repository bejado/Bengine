#include <ConsoleManager.h>

namespace BNG
{

	void ConsoleManager::HandleConsoleInput()
	{
		size_t prevConsoleLength = console.length();
		std::wstring kbuffer = InputManager::Get().ReadKeyboardBuffer();
		console += kbuffer;

		size_t pos = prevConsoleLength;
		while ( pos < console.length() )
		{
			if ( console[pos] == '\b' )
			{
				console.erase( pos, 1 );	// erase the backspace character
				if ( pos > 0 )
				{
					console.erase( pos - 1, 1 );	// and the prior character
					pos--;
				}
			}
			else
			{
				pos++;
			}
		}
	}

	void ConsoleManager::RenderConsole() const
	{
		GraphicsDriver::Get()->SpriteFontBegin();
		GraphicsDriver::Get()->DrawSpriteFontString( console, 0.f, 0.f );
		GraphicsDriver::Get()->SpriteFontEnd();
	}

}