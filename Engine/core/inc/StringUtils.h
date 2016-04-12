namespace StringUtils
{
	wstring GetCommandLineArg( int inIndex );

	wstring Sprintf( const wchar_t* inFormat, ... );

	void	Log( const wchar_t* inFormat );
	void	Log( const wchar_t* inFormat, ... );
}

#define LOG( ... ) StringUtils::Log( __VA_ARGS__ );