#include <PrecompiledHeader.h>
#include <windowsx.h>	// for GET_X_LPARAM

namespace ITP485
{

	void InputManager::SetWindow( HWND wnd )
	{
		RAWINPUTDEVICE mouse;
		mouse.usUsagePage = 0x01;
		mouse.usUsage = 0x02;
		mouse.dwFlags = RIDEV_NOLEGACY;
		mouse.hwndTarget = 0;
		bool result = RegisterRawInputDevices( &mouse, 1, sizeof( mouse ) );
		Dbg_Assert( result, "Failed to register for raw mouse input." );
	}

	void InputManager::ResetEvents()
	{
		mMouseX = 0;
		mMouseY = 0;
	}

	void InputManager::HandleEvent( UINT message, WPARAM inWParam, LPARAM inLParam )
	{
		switch( message )
		{
			case WM_KEYDOWN:
			case WM_KEYUP:
				HandleKeyboardMessage( message, inWParam, inLParam );
				break;

			case WM_INPUT:
				HandleRawInputMessage( message, inWParam, inLParam );
				break;
		}
	}

	bool InputManager::GetKeyState(Key key)
	{
		return mKeyState[key];
	}

	void InputManager::HandleKeyboardMessage( UINT message, WPARAM inWParam, LPARAM inLParam )
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
		if ( inWParam == 0x1B ) {
			mKeyState[Key::ESC] = state;
		}
	}

	void InputManager::HandleRawInputMessage( UINT message, WPARAM inWParam, LPARAM inLParam )
	{
		UINT dwSize;

		GetRawInputData( (HRAWINPUT)inLParam, RID_INPUT, nullptr, &dwSize, sizeof( RAWINPUTHEADER ) );
		LPBYTE lpb = new BYTE[dwSize];
		Dbg_Assert( lpb != nullptr, "Lpb is null" );

		int size = GetRawInputData( (HRAWINPUT)inLParam, RID_INPUT, lpb, &dwSize, sizeof( RAWINPUTHEADER ) );
		Dbg_Assert( size == dwSize, "GetRawInputData does not return correct size !" );

		RAWINPUT* raw = (RAWINPUT*)lpb;

		if (raw->header.dwType == RIM_TYPEMOUSE)
		{
			mMouseX = raw->data.mouse.lLastX;
			mMouseY = raw->data.mouse.lLastY;
		}

		delete[] lpb;
	}
}