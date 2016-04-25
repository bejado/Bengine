#include <PrecompiledHeader.h>

namespace ITP485
{

	void InputManager::HandleEvent( UINT message, WPARAM inWParam, LPARAM inLParam )
	{
		bool state = message == WM_KEYDOWN;

		if ( inWParam == 0x57 ) {
			mKeyState[Key::W] = state;
		}
		if ( inWParam == 0x41 ) {
			mKeyState[Key::A] = state;
		}
		if ( inWParam == 0x53 ) {
			mKeyState[Key::S] = state;
		}
		if ( inWParam == 0x44 ) {
			mKeyState[Key::D] = state;
		}
	}

	bool InputManager::GetKeyState(Key key)
	{
		return mKeyState[key];
	}
}