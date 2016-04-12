#include <PrecompiledHeader.h>

Timing	Timing::sInstance;

Timing::Timing()
{
	LARGE_INTEGER perfFreq;
	QueryPerformanceFrequency( &perfFreq );
	mPerfCountDuration = 1.0 / perfFreq.QuadPart;

	mLastFrameStartTime = GetTime();
}

void Timing::Update()
{

	double currentTime = GetTime();

    mDeltaTime = ( float ) ( currentTime - mLastFrameStartTime );
	/*
	if( mDeltaTime < 0.01f )
	{
		Sleep( ( DWORD ) ( 10.f - mDeltaTime * 1000.f ) );
	}
	*/
	

	mLastFrameStartTime = currentTime;

}

double Timing::GetTime() const
{
	static LARGE_INTEGER startTime = { 0 };

	if ( startTime.QuadPart == 0 )
	{
		QueryPerformanceCounter( &startTime );
		return 0;
	}

		
	LARGE_INTEGER curTime, timeSinceStart;
	QueryPerformanceCounter( &curTime );

	timeSinceStart.QuadPart = curTime.QuadPart - startTime.QuadPart;

	return timeSinceStart.QuadPart * mPerfCountDuration;
}