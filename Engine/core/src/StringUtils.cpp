#include <PrecompiledHeader.h>
#include <stdio.h>


wstring StringUtils::GetCommandLineArg( int inIndex )
{
	if( inIndex < __argc )
	{
		return wstring( __wargv[ inIndex ] );
	}

	return wstring();
}


wstring StringUtils::Sprintf( const wchar_t* inFormat, ... )
{
	//not thread safe...
	static wchar_t temp[ 4096 ];

	va_list args;
	va_start (args, inFormat );

	_vsnwprintf_s( temp, 4096, 4096, inFormat, args );
	return wstring( temp );
}

void StringUtils::Log( const wchar_t* inFormat )
{
	OutputDebugString( inFormat );
	OutputDebugString( L"\n" );
}

void StringUtils::Log( const wchar_t* inFormat, ... )
{
	//not thread safe...
	static wchar_t temp[ 4096 ];

	va_list args;
	va_start (args, inFormat );

	_vsnwprintf_s( temp, 4096, 4096, inFormat, args );
	OutputDebugString( temp );
	OutputDebugString( L"\n" );
}

