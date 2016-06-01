#include "MeshPrimitive.h"

namespace ITP485
{

	class ObjMeshPrimitive : public MeshPrimitive
	{
	public:

		DECLARE_ALIGNED_NEW_DELETE

		ObjMeshPrimitive( std::string file );

	};

}