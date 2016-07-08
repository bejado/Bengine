#pragma once

#include "MeshPrimitive.h"

namespace ITP485
{

	struct RawMesh
	{
		std::vector<uint16_t> indices;
		std::vector<PackedVector3> positions;
		std::vector<PackedVector2> textureCoordinates;
		std::vector<PackedVector3> normals;
		std::vector<PackedVector4> tangents;

		GraphicsBufferPtr CreateVertexBuffer();
		GraphicsBufferPtr CreateIndexBuffer();
	};

	class ObjMeshPrimitive : public MeshPrimitive
	{
	public:

		DECLARE_ALIGNED_NEW_DELETE

		ObjMeshPrimitive( std::string file, MaterialPtr material );

	};

}