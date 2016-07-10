#include "DebugDrawing.h"
#include "WireframeMaterial.h"
#include "Primitives.h"

namespace ITP485
{

	MeshPrimitivePtr CreateDebugSphere( float radius )
	{
		MaterialPtr sphereMaterial = MaterialPtr( new WireframeMaterial() );
		MeshPrimitivePtr sphere = MeshPrimitivePtr( new SpherePrimitive( sphereMaterial ) );
		return sphere;
	}

}
