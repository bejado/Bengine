#pragma once

#include "MeshPrimitive.h"	// TODO: do the integration!

namespace ITP485
{

	// Forward declaration
	struct PrimitiveDrawer::Mesh;

	class Quad 
	{
	public:

		DECLARE_ALIGNED_NEW_DELETE

		Quad( uint32_t instanceDataSize, uint32_t instanceCount );
		void DrawInstanced( uint32_t instanceCount );
		void* MapInstanceBuffer();
		void UnmapInstanceBuffer();

		void FillOutMeshStruct( PrimitiveDrawer::Mesh* const mesh );

	private:

		GraphicsBufferPtr mVertexBuffer;
		GraphicsBufferPtr mIndexBuffer;
		GraphicsBufferPtr mInstanceBuffer;

		VertexShaderPtr mVertexShader;
		InputLayoutPtr mInputLayout;

		uint32_t mInstanceDataSize;
		uint32_t mInstanceCount;

	};

	typedef shared_ptr< Quad > QuadPtr;

}