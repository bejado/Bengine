namespace ITP485
{
	// TODO: we should probably support other keys too
	enum Key
	{
		W = 0,
		S,
		A,
		D,
		ESC,
		NumKeys
	};

	class InputManager : public Singleton < InputManager >
	{
	public:
		DECLARE_SINGLETON( InputManager );

		void SetWindow( HWND wnd );
		void ResetEvents();
		void HandleEvent( UINT message, WPARAM inWParam, LPARAM inLParam );
		bool GetKeyState( Key key );
		long GetMouseX() { return mMouseX; }
		long GetMouseY() { return mMouseY; }

	private:

		void HandleKeyboardMessage( UINT message, WPARAM inWParam, LPARAM inLParam );
		void HandleRawInputMessage( UINT message, WPARAM inWParam, LPARAM inLParam );

		bool mKeyState[NumKeys];
		long mMouseX, mMouseY;

	};

}