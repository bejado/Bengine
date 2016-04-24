// math.h will tell our engine whether or not to use the SIMD library
#ifndef _MATH_H_
#define _MATH_H_

#include "fastmath.h"
namespace ITP485
{
	typedef FastVector3 Vector3;
	typedef FastMatrix4 Matrix4;
	typedef FastQuaternion Quaternion;
}

namespace ITP485
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

	__forceinline Vector3 Dot( const Vector3& inA, const Vector3& inB )
	{
		Vector3 toRet = inA;
		toRet.Dot( inB );
		return toRet;
	}


}

#endif // _MATH_H_
