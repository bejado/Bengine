namespace StringUtils
{
	wstring GetCommandLineArg( int inIndex );

	wstring Sprintf( const wchar_t* inFormat, ... );

	void	Log( const wchar_t* inFormat );
	void	Log( const wchar_t* inFormat, ... );
}

#define BELOG( ... ) StringUtils::Log( __VA_ARGS__ );