// math.h will tell our engine whether or not to use the SIMD library
#ifndef _MATH_H_
#define _MATH_H_

#include "fastmath.h"
namespace BNG
{
	typedef FastVector3 Vector3;
	typedef FastMatrix4 Matrix4;
	typedef FastQuaternion Quaternion;
}

namespace BNG
{
	//helper operator overloads to make using library not terrible.  the clunkyness will all be optimized away...
	__forceinline Vector3 operator+( const Vector3& inA, const Vector3& inB )
	{
		Vector3 toRet = inA;
		toRet.Add( inB );
		return toRet;
	}

	__forceinline Vector3 operator-( const Vector3& inA, const Vector3& inB )
	{
		Vector3 toRet = inA;
		toRet.Sub( inB );
		return toRet;
	}


	__forceinline Vector3 operator*( const Vector3& inA, const float inB )
	{
		Vector3 toRet = inA;
		toRet.Multiply( inB );
		return toRet;
	}

	__forceinline Vector3 operator*( const float inA, const Vector3& inB )
	{
		return operator*( inB, inA );
	}

	__forceinline Vector3 Dot( const Vector3& inA, const Vector3& inB )
	{
		Vector3 toRet = inA;
		toRet.Dot( inB );
		return toRet;
	}

	inline float RandomFloat(float LO, float HI)
	{
		return LO + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (HI - LO)));
	}

}

#endif // _MATH_H_
