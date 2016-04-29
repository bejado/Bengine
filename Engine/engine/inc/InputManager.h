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

		/* Returns true if the InputManager was successfully able to handle the event. */
		bool HandleEvent( UINT message, WPARAM inWParam, LPARAM inLParam );

		bool GetKeyState( Key key );
		long GetMouseDeltaX() { return mMouseX; }
		long GetMouseDeltaY() { return mMouseY; }
		short GetMouseWheelDelta();

	private:

		void HandleKeyboardMessage( UINT message, WPARAM inWParam, LPARAM inLParam );
		void HandleRawInputMessage( UINT message, WPARAM inWParam, LPARAM inLParam );

		bool mKeyState[NumKeys];
		long mMouseX = 0, mMouseY = 0;
		unsigned short mMouseButtonFlags = 0;
		short mMouseButtonData = 0;

	};

}