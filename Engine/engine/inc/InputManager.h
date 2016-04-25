namespace ITP485
{
	// TODO: we should probably support other keys too
	enum Key
	{
		W = 0,
		S,
		A,
		D,
		NumKeys
	};

	class InputManager : public Singleton < InputManager >
	{
	public:
		DECLARE_SINGLETON( InputManager );

		void HandleEvent( UINT message, WPARAM inWParam, LPARAM inLParam );
		bool GetKeyState( Key key );

	private:
		bool mKeyState[NumKeys];

	};

}