#include "DebugDrawing.h"
#include "WireframeMaterial.h"
#include "Primitives.h"

namespace ITP485
{

	MeshPrimitivePtr CreateDebugSphere( float radius )
	{
		MaterialPtr sphereMaterial = MaterialPtr( new WireframeMaterial() );
		RawMeshPtr sphereMesh = RawMeshPtr( new RawMesh() );
		GenerateSphereVerticies( *sphereMesh.get() );
		MeshPrimitivePtr sphere = MeshPrimitivePtr( new MeshPrimitive( sphereMesh, sphereMaterial, GetPositionOnlyVertexSource() ) );
		return sphere;
	}

}
