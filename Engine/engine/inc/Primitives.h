#include "MeshPrimitive.h"

namespace ITP485
{

	class CubePrimitive : public MeshPrimitive
	{
	public:

		DECLARE_ALIGNED_NEW_DELETE

		CubePrimitive();

	};

	class QuadPrimitive : public MeshPrimitive
	{
	public:

		QuadPrimitive();

	};

	class SpherePrimitive : public MeshPrimitive
	{
	public:

		SpherePrimitive( MaterialPtr material );

	};

}