#pragma once

#include "MeshPrimitive.h"
#include <PrecompiledHeader.h>

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
		GraphicsBufferPtr CreatePositionIndexOnlyVertexBuffer();
		GraphicsBufferPtr CreateIndexBuffer();
	};

}