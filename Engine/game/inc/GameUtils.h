#include <PrecompiledHeader.h>

namespace ITP485
{
	// Allow ESC to exit
	inline void ExitWithEscapeKey()
	{
		if ( InputManager::Get().GetKeyState( Key::ESC ) ) {
			PostQuitMessage( 0 );
		}
	}
}
